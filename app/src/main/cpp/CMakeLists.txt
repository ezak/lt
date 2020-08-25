# For more information about using CMake with Android Studio, read the
# documentation: https://d.android.com/studio/projects/add-native-code.html

# Sets the minimum version of CMake required to build the native library.

cmake_minimum_required(VERSION 3.4.1)

# configure import libs
set(libtorrent_DIR /home/izak/libtorrent-android-bin)

add_library(libtorrent STATIC IMPORTED)
set_target_properties(libtorrent PROPERTIES IMPORTED_LOCATION
        ${libtorrent_DIR}/${ANDROID_ABI}/lib/libtorrent-rasterbar.a)

add_library(libboost_atomic STATIC IMPORTED)
set_target_properties(libboost_atomic PROPERTIES IMPORTED_LOCATION
        ${libtorrent_DIR}/${ANDROID_ABI}/lib/libboost_atomic-clang-mt-a64-1_73.a)

add_library(libboost_chrono STATIC IMPORTED)
set_target_properties(libboost_chrono PROPERTIES IMPORTED_LOCATION
        ${libtorrent_DIR}/${ANDROID_ABI}/lib/libboost_chrono-clang-mt-a64-1_73.a)

add_library(libboost_filesystem STATIC IMPORTED)
set_target_properties(libboost_filesystem PROPERTIES IMPORTED_LOCATION
        ${libtorrent_DIR}/${ANDROID_ABI}/lib/libboost_filesystem-clang-mt-a64-1_73.a)

add_library(libboost_random STATIC IMPORTED)
set_target_properties(libboost_random PROPERTIES IMPORTED_LOCATION
        ${libtorrent_DIR}/${ANDROID_ABI}/lib/libboost_random-clang-mt-a64-1_73.a)

add_library(libboost_system STATIC IMPORTED)
set_target_properties(libboost_system PROPERTIES IMPORTED_LOCATION
        ${libtorrent_DIR}/${ANDROID_ABI}/lib/libboost_system-clang-mt-a64-1_73.a)

add_library(libcrypto STATIC IMPORTED)
set_target_properties(libcrypto PROPERTIES IMPORTED_LOCATION
        ${libtorrent_DIR}/${ANDROID_ABI}/lib/libcrypto.a)

add_library(libssl STATIC IMPORTED)
set_target_properties(libssl PROPERTIES IMPORTED_LOCATION
        ${libtorrent_DIR}/${ANDROID_ABI}/lib/libssl.a)

# build application's shared lib
#set(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} -std=gnu++11")

add_library(client_test SHARED
	altc.cpp)

target_include_directories(client_test PRIVATE
        ${libtorrent_DIR}/${ANDROID_ABI}/include)

target_link_libraries(client_test
        libtorrent
        libboost_atomic
        libboost_chrono
        libboost_filesystem
        libboost_random
        libboost_system
        libssl
        libcrypto
        log)
