# CMAKE generated file: DO NOT EDIT!
# Generated by "Unix Makefiles" Generator, CMake Version 3.16

# Delete rule output on recipe failure.
.DELETE_ON_ERROR:


#=============================================================================
# Special targets provided by cmake.

# Disable implicit rules so canonical targets will work.
.SUFFIXES:


# Remove some rules from gmake that .SUFFIXES does not remove.
SUFFIXES =

.SUFFIXES: .hpux_make_needs_suffix_list


# Suppress display of executed commands.
$(VERBOSE).SILENT:


# A target that is always out of date.
cmake_force:

.PHONY : cmake_force

#=============================================================================
# Set environment variables for the build.

# The shell in which to execute make rules.
SHELL = /bin/sh

# The CMake executable.
CMAKE_COMMAND = /home/izak/bin/clion-2020.1.1/bin/cmake/linux/bin/cmake

# The command to remove a file.
RM = /home/izak/bin/clion-2020.1.1/bin/cmake/linux/bin/cmake -E remove -f

# Escaping for special characters.
EQUALS = =

# The top-level source directory on which CMake was run.
CMAKE_SOURCE_DIR = /home/izak/random/lt/app/src/main/cpp

# The top-level build directory on which CMake was run.
CMAKE_BINARY_DIR = /home/izak/random/lt/app/src/main/cpp/cmake-build-debug

# Include any dependencies generated for this target.
include CMakeFiles/client_test.dir/depend.make

# Include the progress variables for this target.
include CMakeFiles/client_test.dir/progress.make

# Include the compile flags for this target's objects.
include CMakeFiles/client_test.dir/flags.make

CMakeFiles/client_test.dir/ltclient.cpp.o: CMakeFiles/client_test.dir/flags.make
CMakeFiles/client_test.dir/ltclient.cpp.o: ../ltclient.cpp
	@$(CMAKE_COMMAND) -E cmake_echo_color --switch=$(COLOR) --green --progress-dir=/home/izak/random/lt/app/src/main/cpp/cmake-build-debug/CMakeFiles --progress-num=$(CMAKE_PROGRESS_1) "Building CXX object CMakeFiles/client_test.dir/ltclient.cpp.o"
	/usr/bin/c++  $(CXX_DEFINES) $(CXX_INCLUDES) $(CXX_FLAGS) -o CMakeFiles/client_test.dir/ltclient.cpp.o -c /home/izak/random/lt/app/src/main/cpp/ltclient.cpp

CMakeFiles/client_test.dir/ltclient.cpp.i: cmake_force
	@$(CMAKE_COMMAND) -E cmake_echo_color --switch=$(COLOR) --green "Preprocessing CXX source to CMakeFiles/client_test.dir/ltclient.cpp.i"
	/usr/bin/c++ $(CXX_DEFINES) $(CXX_INCLUDES) $(CXX_FLAGS) -E /home/izak/random/lt/app/src/main/cpp/ltclient.cpp > CMakeFiles/client_test.dir/ltclient.cpp.i

CMakeFiles/client_test.dir/ltclient.cpp.s: cmake_force
	@$(CMAKE_COMMAND) -E cmake_echo_color --switch=$(COLOR) --green "Compiling CXX source to assembly CMakeFiles/client_test.dir/ltclient.cpp.s"
	/usr/bin/c++ $(CXX_DEFINES) $(CXX_INCLUDES) $(CXX_FLAGS) -S /home/izak/random/lt/app/src/main/cpp/ltclient.cpp -o CMakeFiles/client_test.dir/ltclient.cpp.s

CMakeFiles/client_test.dir/session_view.cpp.o: CMakeFiles/client_test.dir/flags.make
CMakeFiles/client_test.dir/session_view.cpp.o: ../session_view.cpp
	@$(CMAKE_COMMAND) -E cmake_echo_color --switch=$(COLOR) --green --progress-dir=/home/izak/random/lt/app/src/main/cpp/cmake-build-debug/CMakeFiles --progress-num=$(CMAKE_PROGRESS_2) "Building CXX object CMakeFiles/client_test.dir/session_view.cpp.o"
	/usr/bin/c++  $(CXX_DEFINES) $(CXX_INCLUDES) $(CXX_FLAGS) -o CMakeFiles/client_test.dir/session_view.cpp.o -c /home/izak/random/lt/app/src/main/cpp/session_view.cpp

CMakeFiles/client_test.dir/session_view.cpp.i: cmake_force
	@$(CMAKE_COMMAND) -E cmake_echo_color --switch=$(COLOR) --green "Preprocessing CXX source to CMakeFiles/client_test.dir/session_view.cpp.i"
	/usr/bin/c++ $(CXX_DEFINES) $(CXX_INCLUDES) $(CXX_FLAGS) -E /home/izak/random/lt/app/src/main/cpp/session_view.cpp > CMakeFiles/client_test.dir/session_view.cpp.i

CMakeFiles/client_test.dir/session_view.cpp.s: cmake_force
	@$(CMAKE_COMMAND) -E cmake_echo_color --switch=$(COLOR) --green "Compiling CXX source to assembly CMakeFiles/client_test.dir/session_view.cpp.s"
	/usr/bin/c++ $(CXX_DEFINES) $(CXX_INCLUDES) $(CXX_FLAGS) -S /home/izak/random/lt/app/src/main/cpp/session_view.cpp -o CMakeFiles/client_test.dir/session_view.cpp.s

CMakeFiles/client_test.dir/torrent_view.cpp.o: CMakeFiles/client_test.dir/flags.make
CMakeFiles/client_test.dir/torrent_view.cpp.o: ../torrent_view.cpp
	@$(CMAKE_COMMAND) -E cmake_echo_color --switch=$(COLOR) --green --progress-dir=/home/izak/random/lt/app/src/main/cpp/cmake-build-debug/CMakeFiles --progress-num=$(CMAKE_PROGRESS_3) "Building CXX object CMakeFiles/client_test.dir/torrent_view.cpp.o"
	/usr/bin/c++  $(CXX_DEFINES) $(CXX_INCLUDES) $(CXX_FLAGS) -o CMakeFiles/client_test.dir/torrent_view.cpp.o -c /home/izak/random/lt/app/src/main/cpp/torrent_view.cpp

CMakeFiles/client_test.dir/torrent_view.cpp.i: cmake_force
	@$(CMAKE_COMMAND) -E cmake_echo_color --switch=$(COLOR) --green "Preprocessing CXX source to CMakeFiles/client_test.dir/torrent_view.cpp.i"
	/usr/bin/c++ $(CXX_DEFINES) $(CXX_INCLUDES) $(CXX_FLAGS) -E /home/izak/random/lt/app/src/main/cpp/torrent_view.cpp > CMakeFiles/client_test.dir/torrent_view.cpp.i

CMakeFiles/client_test.dir/torrent_view.cpp.s: cmake_force
	@$(CMAKE_COMMAND) -E cmake_echo_color --switch=$(COLOR) --green "Compiling CXX source to assembly CMakeFiles/client_test.dir/torrent_view.cpp.s"
	/usr/bin/c++ $(CXX_DEFINES) $(CXX_INCLUDES) $(CXX_FLAGS) -S /home/izak/random/lt/app/src/main/cpp/torrent_view.cpp -o CMakeFiles/client_test.dir/torrent_view.cpp.s

CMakeFiles/client_test.dir/print.cpp.o: CMakeFiles/client_test.dir/flags.make
CMakeFiles/client_test.dir/print.cpp.o: ../print.cpp
	@$(CMAKE_COMMAND) -E cmake_echo_color --switch=$(COLOR) --green --progress-dir=/home/izak/random/lt/app/src/main/cpp/cmake-build-debug/CMakeFiles --progress-num=$(CMAKE_PROGRESS_4) "Building CXX object CMakeFiles/client_test.dir/print.cpp.o"
	/usr/bin/c++  $(CXX_DEFINES) $(CXX_INCLUDES) $(CXX_FLAGS) -o CMakeFiles/client_test.dir/print.cpp.o -c /home/izak/random/lt/app/src/main/cpp/print.cpp

CMakeFiles/client_test.dir/print.cpp.i: cmake_force
	@$(CMAKE_COMMAND) -E cmake_echo_color --switch=$(COLOR) --green "Preprocessing CXX source to CMakeFiles/client_test.dir/print.cpp.i"
	/usr/bin/c++ $(CXX_DEFINES) $(CXX_INCLUDES) $(CXX_FLAGS) -E /home/izak/random/lt/app/src/main/cpp/print.cpp > CMakeFiles/client_test.dir/print.cpp.i

CMakeFiles/client_test.dir/print.cpp.s: cmake_force
	@$(CMAKE_COMMAND) -E cmake_echo_color --switch=$(COLOR) --green "Compiling CXX source to assembly CMakeFiles/client_test.dir/print.cpp.s"
	/usr/bin/c++ $(CXX_DEFINES) $(CXX_INCLUDES) $(CXX_FLAGS) -S /home/izak/random/lt/app/src/main/cpp/print.cpp -o CMakeFiles/client_test.dir/print.cpp.s

# Object files for target client_test
client_test_OBJECTS = \
"CMakeFiles/client_test.dir/ltclient.cpp.o" \
"CMakeFiles/client_test.dir/session_view.cpp.o" \
"CMakeFiles/client_test.dir/torrent_view.cpp.o" \
"CMakeFiles/client_test.dir/print.cpp.o"

# External object files for target client_test
client_test_EXTERNAL_OBJECTS =

libclient_test.so: CMakeFiles/client_test.dir/ltclient.cpp.o
libclient_test.so: CMakeFiles/client_test.dir/session_view.cpp.o
libclient_test.so: CMakeFiles/client_test.dir/torrent_view.cpp.o
libclient_test.so: CMakeFiles/client_test.dir/print.cpp.o
libclient_test.so: CMakeFiles/client_test.dir/build.make
libclient_test.so: /home/izak/libtorrent-android-bin/lib/libtorrent-rasterbar.a
libclient_test.so: /home/izak/libtorrent-android-bin/lib/libboost_atomic-clang-mt-a64-1_73.a
libclient_test.so: /home/izak/libtorrent-android-bin/lib/libboost_chrono-clang-mt-a64-1_73.a
libclient_test.so: /home/izak/libtorrent-android-bin/lib/libboost_filesystem-clang-mt-a64-1_73.a
libclient_test.so: /home/izak/libtorrent-android-bin/lib/libboost_random-clang-mt-a64-1_73.a
libclient_test.so: /home/izak/libtorrent-android-bin/lib/libboost_system-clang-mt-a64-1_73.a
libclient_test.so: /home/izak/libtorrent-android-bin/lib/libssl.a
libclient_test.so: /home/izak/libtorrent-android-bin/lib/libcrypto.a
libclient_test.so: CMakeFiles/client_test.dir/link.txt
	@$(CMAKE_COMMAND) -E cmake_echo_color --switch=$(COLOR) --green --bold --progress-dir=/home/izak/random/lt/app/src/main/cpp/cmake-build-debug/CMakeFiles --progress-num=$(CMAKE_PROGRESS_5) "Linking CXX shared library libclient_test.so"
	$(CMAKE_COMMAND) -E cmake_link_script CMakeFiles/client_test.dir/link.txt --verbose=$(VERBOSE)

# Rule to build all files generated by this target.
CMakeFiles/client_test.dir/build: libclient_test.so

.PHONY : CMakeFiles/client_test.dir/build

CMakeFiles/client_test.dir/clean:
	$(CMAKE_COMMAND) -P CMakeFiles/client_test.dir/cmake_clean.cmake
.PHONY : CMakeFiles/client_test.dir/clean

CMakeFiles/client_test.dir/depend:
	cd /home/izak/random/lt/app/src/main/cpp/cmake-build-debug && $(CMAKE_COMMAND) -E cmake_depends "Unix Makefiles" /home/izak/random/lt/app/src/main/cpp /home/izak/random/lt/app/src/main/cpp /home/izak/random/lt/app/src/main/cpp/cmake-build-debug /home/izak/random/lt/app/src/main/cpp/cmake-build-debug /home/izak/random/lt/app/src/main/cpp/cmake-build-debug/CMakeFiles/client_test.dir/DependInfo.cmake --color=$(COLOR)
.PHONY : CMakeFiles/client_test.dir/depend
