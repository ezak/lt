//
// Created by izak on 5/28/20.
//

#include <jni.h>
#include <cstring>
#include <libtorrent/session.hpp>
#include <libtorrent/bencode.hpp>
#include <libtorrent/read_resume_data.hpp>
#include <libtorrent/torrent_status.hpp>
#include <libtorrent/magnet_uri.hpp>
#include <libtorrent/torrent_info.hpp>
#include <libtorrent/alert_types.hpp>


#include <android/log.h>
#define LOG_TAG "~ltclient"
#define logErr(...) __android_log_print(ANDROID_LOG_ERROR, LOG_TAG, __VA_ARGS__)

int num_outstanding_resume_data = 0;
bool quit = false;


lt::storage_mode_t allocation_mode = lt::storage_mode_sparse;
std::string save_path(".");
int torrent_upload_limit = 0;
int torrent_download_limit = 0;
std::string monitor_dir;
int poll_interval = 5;
int max_connections_per_torrent = 50;
bool seed_mode = false;
bool stats_enabled = false;
int cache_size = -1;

bool share_mode = false;
bool disable_storage = false;


std::string to_hex(lt::sha1_hash const& s)
{
    std::stringstream ret;
    ret << s;
    return ret.str();
}


int save_file(std::string const& filename, std::vector<char> const& v)
{
    std::fstream f(filename, std::ios_base::trunc | std::ios_base::out | std::ios_base::binary);
    f.write(v.data(), v.size());
    return !f.fail();
}

std::string path_append(std::string const& lhs, std::string const& rhs)
{
    if (lhs.empty() || lhs == ".") return rhs;
    if (rhs.empty() || rhs == ".") return lhs;

#define TORRENT_SEPARATOR "/"
    bool need_sep = lhs[lhs.size()-1] != '/';

    return lhs + (need_sep?TORRENT_SEPARATOR:"") + rhs;
}

std::vector<std::string> list_dir(std::string path
        , bool (*filter_fun)(lt::string_view)
        , lt::error_code& ec)
{
    std::vector<std::string> ret;

    if (!path.empty() && path[path.size()-1] == '/')
        path.resize(path.size()-1);

    DIR* handle = opendir(path.c_str());
    if (handle == nullptr)
    {
        ec.assign(errno, boost::system::system_category());
        return ret;
    }

    struct dirent* de;
    while ((de = readdir(handle)))
    {
        lt::string_view p(de->d_name);
        if (filter_fun(p))
            ret.push_back(p.to_string());
    }
    closedir(handle);

    return ret;
}


bool load_file(std::string const& filename, std::vector<char>& v
        , int limit = 8000000)
{
    std::fstream f(filename, std::ios_base::in | std::ios_base::binary);
    f.seekg(0, std::ios_base::end);
    auto const s = f.tellg();
    if (s > limit || s < 0) return false;
    f.seekg(0, std::ios_base::beg);
    v.resize(static_cast<std::size_t>(s));
    if (s == std::fstream::pos_type(0)) return !f.fail();
    f.read(v.data(), v.size());
    return !f.fail();
}

std::string resume_file(lt::sha1_hash const& info_hash)
{
    return path_append(save_path, path_append(".resume"
            , to_hex(info_hash) + ".resume"));
}


bool is_resume_file(std::string const& s)
{
    static std::string const hex_digit = "0123456789abcdef";
    if (s.size() != 40 + 7) return false;
    if (s.substr(40) != ".resume") return false;
    for (char const c : s.substr(0, 40))
    {
        if (hex_digit.find(c) == std::string::npos) return false;
    }
    return true;
}

void set_torrent_params(lt::add_torrent_params& p)
{
    p.max_connections = max_connections_per_torrent;
    p.max_uploads = -1;
    p.upload_limit = torrent_upload_limit;
    p.download_limit = torrent_download_limit;

    if (seed_mode) p.flags |= lt::torrent_flags::seed_mode;
    if (disable_storage) p.storage = lt::disabled_storage_constructor;
    if (share_mode) p.flags |= lt::torrent_flags::share_mode;
    p.save_path = save_path;
    p.storage_mode = allocation_mode;
}

void add_magnet(lt::session& ses, lt::string_view uri)
{
    lt::error_code ec;
    lt::add_torrent_params p = lt::parse_magnet_uri(uri.to_string(), ec);

    if (ec)
    {
        logErr("invalid magnet link \"%s\": %s\n"
        , uri.to_string().c_str(), ec.message().c_str());
        return;
    }

    std::vector<char> resume_data;
    if (load_file(resume_file(p.info_hash), resume_data))
    {
        p = lt::read_resume_data(resume_data, ec);
        if (ec) logErr("  failed to load resume data: %s\n", ec.message().c_str());
    }


    set_torrent_params(p);


    logErr("adding magnet: %s\n", uri.to_string().c_str());
    ses.async_add_torrent(std::move(p));
}

// return false on failure
bool add_torrent(lt::session& ses, std::string torrent)
{
    static int counter = 0;

    logErr("[%d] %s\n", counter++, torrent.c_str());

    lt::error_code ec;
    auto ti = std::make_shared<lt::torrent_info>(torrent, ec);
    if (ec)
    {
        logErr("failed to load torrent \"%s\": %s\n"
        , torrent.c_str(), ec.message().c_str());
        return false;
    }

    lt::add_torrent_params p;

    std::vector<char> resume_data;
    if (load_file(resume_file(ti->info_hash()), resume_data))
    {
        p = lt::read_resume_data(resume_data, ec);
        if (ec) logErr("  failed to load resume data: %s\n", ec.message().c_str());
    }

    set_torrent_params(p);

    p.ti = ti;
    p.flags &= ~lt::torrent_flags::duplicate_is_error;
    ses.async_add_torrent(std::move(p));
    return true;
}

void scan_dir(std::string const& dir_path, lt::session& ses)
{
    lt::error_code ec;
    std::vector<std::string> ents = list_dir(dir_path
            , [](lt::string_view p) { return p.size() > 8 && p.substr(p.size() - 8) == ".torrent"; }, ec);
    if (ec)
    {
        std::fprintf(stderr, "failed to list directory: (%s : %d) %s\n"
                , ec.category().name(), ec.value(), ec.message().c_str());
        return;
    }

    for (auto const& e : ents)
    {
        std::string const file = path_append(dir_path, e);

        // there's a new file in the monitor directory, load it up
        if (add_torrent(ses, file))
        {
            if (::remove(file.c_str()) < 0)
            {
                std::fprintf(stderr, "failed to remove torrent file: \"%s\"\n"
                        , file.c_str());
            }
        }
    }
}

char const* timestamp()
{
    time_t t = std::time(nullptr);
    tm* timeinfo = std::localtime(&t);
    static char str[200];
    std::strftime(str, 200, "%b %d %X", timeinfo);
    return str;
}

// return the name of a torrent status enum
char const* state(lt::torrent_status::state_t s)
{
    switch(s) {
        case lt::torrent_status::checking_files: return "checking";
        case lt::torrent_status::downloading_metadata: return "dl metadata";
        case lt::torrent_status::downloading: return "downloading";
        case lt::torrent_status::finished: return "finished";
        case lt::torrent_status::seeding: return "seeding";
        case lt::torrent_status::allocating: return "allocating";
        case lt::torrent_status::checking_resume_data: return "checking resume";
        default: return "<>";
    }
}


// processing callback to handler class
typedef struct my_context {
    JavaVM  *javaVM;
    jclass   jniHelperClz;
    jobject  jniHelperObj;
    jmethodID jniSendSomeLogFunc;
} MyContext;
MyContext g_ctx;


JNIEXPORT jint JNICALL JNI_OnLoad(JavaVM* vm, void*reserved)
{
    JNIEnv *env;
    memset(&g_ctx, 0, sizeof(g_ctx));
    g_ctx.javaVM = vm;

    int status = (vm)->GetEnv(reinterpret_cast<void **>(&env), JNI_VERSION_1_6);

    if (status != JNI_OK)
    {
        return JNI_ERR;
    }

    jclass  clz = (env)->FindClass("com/iz/blackwater/lt/JniHandler");
    g_ctx.jniHelperClz = static_cast<jclass>((env)->NewGlobalRef(clz));
    jmethodID jniHelperCtor = (env)->GetMethodID(g_ctx.jniHelperClz, "<init>", "()V");
    jobject handler = (env)->NewObject(g_ctx.jniHelperClz, jniHelperCtor);
    g_ctx.jniHelperObj = (env)->NewGlobalRef(handler);

    g_ctx.jniSendSomeLogFunc = env->GetStaticMethodID(g_ctx.jniHelperClz, "logSomeMesage", "(FLjava/lang/String;Ljava/lang/String;Ljava/lang/String;)V");
    return JNI_VERSION_1_6;
}

void callback_test(lt::session& session, std::vector<lt::torrent_status> &s){
    JNIEnv *env;

    std::vector<lt::torrent_handle> torrentHandle = session.get_torrents();

    int status = g_ctx.javaVM->GetEnv((void**)&env, JNI_VERSION_1_6);

    if (status == JNI_EDETACHED) {
        logErr("Attaching to the Current thread");
        status = g_ctx.javaVM->AttachCurrentThread(&env, nullptr);
    }

    if (status == JNI_OK) {

        jmethodID  sendSomeLog = g_ctx.jniSendSomeLogFunc;


        lt::sha1_hash sha1Hash;
        for (lt::torrent_status st : s){
            jfloat  progress = st.progress;
            jstring fileName = (env)->NewStringUTF(st.name.c_str());
            jstring fileStatus = (env)->NewStringUTF(state(st.state));
            jstring infoHash = (env)->NewStringUTF(to_hex(st.info_hash).c_str());

            env->CallStaticVoidMethod(g_ctx.jniHelperClz, sendSomeLog, progress, fileName, fileStatus, infoHash);
        }


        logErr("Detaching from the current threadl");
        g_ctx.javaVM->DetachCurrentThread();
        return;
    }

    logErr("something went wrong");


}

extern "C"
JNIEXPORT jlong JNICALL
Java_com_iz_blackwater_lt_LibtorrentService_startLibtorrent(JNIEnv *env, jclass clazz){
    jlong jsession = 0 ;
    lt::session *session = 0 ;
    lt::session_params params;

    // prepare settings
#ifndef TORRENT_DISABLE_DHT
    params.dht_settings.privacy_lookups = true;
    std::vector<char> in;
    if (load_file(".ses_state", in))
    {
        lt::error_code ec;
        lt::bdecode_node e = lt::bdecode(in, ec);
        if (!ec) params = lt::read_session_params(e, lt::session_handle::save_dht_state);
    }
#endif

    auto& settings = params.settings;
    settings.set_str(lt::settings_pack::peer_fingerprint, "ABX7S756D3LYGOTZV4VZFSNM75GUR5XFLSMLWJSJPRQDVEWPNZG2TLR7");
    settings.set_str(lt::settings_pack::user_agent, "client_test/" LIBTORRENT_VERSION);
    settings.set_int(lt::settings_pack::cache_size, cache_size);
    settings.set_int(lt::settings_pack::choking_algorithm, lt::settings_pack::rate_based_choker);
    settings.set_int(lt::settings_pack::alert_mask
            , lt::alert_category::error
              | lt::alert_category::peer
              | lt::alert_category::port_mapping
              | lt::alert_category::storage
              | lt::alert_category::tracker
              | lt::alert_category::connect
              | lt::alert_category::status
              | lt::alert_category::ip_block
              | lt::alert_category::performance_warning
              | lt::alert_category::dht
              | lt::alert_category::incoming_request
              | lt::alert_category::dht_operation
              | lt::alert_category::port_mapping_log
              | lt::alert_category::file_progress);


    // create directory for resume files
    int mkdir_ret = mkdir(path_append(save_path, ".resume").c_str(), 0777);
    if (mkdir_ret < 0 && errno != EEXIST)
    {
        std::fprintf(stderr, "failed to create resume file directory: (%d) %s\n"
                , errno, strerror(errno));
    }

    session = (libtorrent::session *)new libtorrent::session(std::move(params));

    std::vector<lt::string_view> torrents;
    for (auto const& i : torrents)
    {
        if (i.substr(0, 7) == "magnet:") add_magnet(*session, i);
        else add_torrent(*session, i.to_string());
    }


    // resume data loop
    std::thread resume_data_loader_thread([session]{
        lt::error_code ec;
        std::string const resume_dir = path_append(save_path, ".resume");
        std::vector<std::string> ents = list_dir(resume_dir , [](lt::string_view p) { return p.size() > 7 && p.substr(p.size() - 7) == ".resume"; }, ec);
        if (ec)
        {
            std::fprintf(stderr, "failed to list resume directory \"%s\": (%s : %d) %s\n"
                    , resume_dir.c_str(), ec.category().name(), ec.value(), ec.message().c_str());
        }
        else
        {
            for (auto const& e : ents)
            {
                // only load resume files of the form <info-hash>.resume
                if (!is_resume_file(e)) continue;
                std::string const file = path_append(resume_dir, e);

                std::vector<char> resume_data;
                if (!load_file(file, resume_data))
                {
                    // logErr("  failed to load resume file \"%s\": %s\n" , file.c_str(), ec.message().c_str());
                    continue;
                }
                lt::add_torrent_params p = lt::read_resume_data(resume_data, ec);
                if (ec)
                {
                    // logErr("  failed to parse resume data \"%s\": %s\n" , file.c_str(), ec.message().c_str());
                    continue;
                }
                session->async_add_torrent(std::move(p));
                std::vector<lt::torrent_handle> torrentHandle = session->get_torrents();
            }
        }
    });
    resume_data_loader_thread.join();



    // main loop
    std::thread main_loop_thread([session, env]{
        std::deque<std::string> events;
        lt::time_point next_dir_scan = lt::clock_type::now();

        quit = false;
        while (!quit)
        {
            session->post_session_stats();
            session->post_torrent_updates();
            session->post_dht_stats();


            std::string out;

            lt::time_point const now = lt::clock_type::now();


            for (auto const& e : events)
            {
                out += e;
            }


            logErr("Libtorrent Events %s", out.c_str());

            if (!monitor_dir.empty() && next_dir_scan < now)
            {
                scan_dir(monitor_dir, *session);
                next_dir_scan = now + lt::seconds(poll_interval);
            }


            std::vector<lt::alert*> alerts;
            session->pop_alerts(&alerts);

            for (lt::alert const* a : alerts) {

                logErr("Libtorrent Alerts %s", a->message().c_str());

                if (auto st = lt::alert_cast<lt::state_update_alert>(a)) {
                    if (st->status.empty()) continue;


                    std::vector<lt::torrent_status> s = st->status;

                    ::callback_test(*session, s);

                }


            }




            std::this_thread::sleep_for(lt::milliseconds(2000));

        }
    });
    main_loop_thread.detach();

    *(libtorrent::session **)&jsession = session;
    return jsession;
}


extern "C"
JNIEXPORT void JNICALL
Java_com_iz_blackwater_lt_LibtorrentService_stopLibtorrent(JNIEnv *env, jclass clazz, jlong jsession){
    quit = true;
    libtorrent::session *session = (libtorrent::session *) 0 ;
    session = *(libtorrent::session **)&jsession;
    session->pause();
    /// logErr("saving resume data\n");

    // get all the torrent handles that we need to save resume data for
    std::vector<lt::torrent_status> const temp = session->get_torrent_status(
            [](lt::torrent_status const& st)
            {
                if (!st.handle.is_valid() || !st.has_metadata || !st.need_save_resume)
                    return false;
                return true;
            }, {});


    int idx = 0;
    for (auto const& st : temp)
    {
        // save_resume_data will generate an alert when it's done
        st.handle.save_resume_data(lt::torrent_handle::save_info_dict);
        ++num_outstanding_resume_data;
        ++idx;
        if ((idx % 32) == 0)
        {
            logErr("\r%d  ", num_outstanding_resume_data);

        }
    }

    logErr("\nwaiting for resume data [%d]\n", num_outstanding_resume_data);

    while (num_outstanding_resume_data > 0)
    {
        lt::alert const* a = session->wait_for_alert(lt::seconds(10));
        if (a == nullptr) continue;

    }

#ifndef TORRENT_DISABLE_DHT
    logErr("\nsaving session state\n");
    {
        lt::entry session_state;
        session->save_state(session_state, lt::session::save_dht_state);

        std::vector<char> out;
        bencode(std::back_inserter(out), session_state);
        save_file(".ses_state", out);
    }
#endif

    logErr("closing session\n");

}


extern "C"
JNIEXPORT void JNICALL
Java_com_iz_blackwater_lt_LibtorrentService_addMagnet(JNIEnv *env, jclass clazz, jlong jsession,
                                                      jstring juri) {
    libtorrent::session *session = (libtorrent::session *) 0 ;
    session = *(libtorrent::session **)&jsession;

    const char * uri =  (env)->GetStringUTFChars(juri, 0);

    logErr("The torrent file is %s", uri);

    add_torrent(*session, uri);

    (env)->ReleaseStringUTFChars(juri, uri);
}