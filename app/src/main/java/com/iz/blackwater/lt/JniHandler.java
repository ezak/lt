package com.iz.blackwater.lt;

import android.os.Build;
import android.util.Log;

import androidx.annotation.Keep;

import com.iz.blackwater.lt.database.AppDatabase;
import com.iz.blackwater.lt.database.TorrentEntry;

import java.util.ArrayList;
import java.util.List;

/*
 * A helper class to demo that JNI could call into:
 *     private non-static function
 *     public non-static function
 *     static public function
 * The calling code is inside hello-jnicallback.c
 */
public class JniHandler {


	static AppDatabase appDatabase = AppDatabase.getInstance(MyApplication.getAppContext());

	/*
	 * Print out status to logcat
	 */
	@Keep
	private void updateStatus(String msg) {
		if (msg.toLowerCase().contains("error")) {
			Log.e("JniHandler", "Native Err: " + msg);
		} else {
			Log.i("JniHandler", "Native Msg: " + msg);
		}
	}

	/*
	 * Return OS build version: a static function
	 */
	@Keep
	static public String getBuildVersion() {
		return Build.VERSION.RELEASE;
	}

	/*
	 * Return Java memory info
	 */
	@Keep
	public long getRuntimeMemorySize() {
		return Runtime.getRuntime().freeMemory();
	}


	public static void logSomeMesage(float progress, String name, String status, final String infoHash){
		Log.d("TAG", ": " + name +  " Progress : " + progress + " Status: " + status);

		final TorrentEntry t = new TorrentEntry(infoHash, name, status, progress);

		AppExecutors.getInstance().diskIO().execute(new Runnable() {
			@Override
			public void run() {
				// if not record insert else replace
				appDatabase.torrentDao().insertTorrent(t);

			}
		});

	}
}

