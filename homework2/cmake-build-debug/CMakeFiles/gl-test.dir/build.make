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
CMAKE_COMMAND = /usr/bin/cmake

# The command to remove a file.
RM = /usr/bin/cmake -E remove -f

# Escaping for special characters.
EQUALS = =

# The top-level source directory on which CMake was run.
CMAKE_SOURCE_DIR = /tmp/tmp.TJwn1KKsci

# The top-level build directory on which CMake was run.
CMAKE_BINARY_DIR = /tmp/tmp.TJwn1KKsci/cmake-build-debug

# Include any dependencies generated for this target.
include CMakeFiles/gl-test.dir/depend.make

# Include the progress variables for this target.
include CMakeFiles/gl-test.dir/progress.make

# Include the compile flags for this target's objects.
include CMakeFiles/gl-test.dir/flags.make

CMakeFiles/gl-test.dir/test/test.cpp.o: CMakeFiles/gl-test.dir/flags.make
CMakeFiles/gl-test.dir/test/test.cpp.o: ../test/test.cpp
	@$(CMAKE_COMMAND) -E cmake_echo_color --switch=$(COLOR) --green --progress-dir=/tmp/tmp.TJwn1KKsci/cmake-build-debug/CMakeFiles --progress-num=$(CMAKE_PROGRESS_1) "Building CXX object CMakeFiles/gl-test.dir/test/test.cpp.o"
	/usr/bin/c++  $(CXX_DEFINES) $(CXX_INCLUDES) $(CXX_FLAGS) -o CMakeFiles/gl-test.dir/test/test.cpp.o -c /tmp/tmp.TJwn1KKsci/test/test.cpp

CMakeFiles/gl-test.dir/test/test.cpp.i: cmake_force
	@$(CMAKE_COMMAND) -E cmake_echo_color --switch=$(COLOR) --green "Preprocessing CXX source to CMakeFiles/gl-test.dir/test/test.cpp.i"
	/usr/bin/c++ $(CXX_DEFINES) $(CXX_INCLUDES) $(CXX_FLAGS) -E /tmp/tmp.TJwn1KKsci/test/test.cpp > CMakeFiles/gl-test.dir/test/test.cpp.i

CMakeFiles/gl-test.dir/test/test.cpp.s: cmake_force
	@$(CMAKE_COMMAND) -E cmake_echo_color --switch=$(COLOR) --green "Compiling CXX source to assembly CMakeFiles/gl-test.dir/test/test.cpp.s"
	/usr/bin/c++ $(CXX_DEFINES) $(CXX_INCLUDES) $(CXX_FLAGS) -S /tmp/tmp.TJwn1KKsci/test/test.cpp -o CMakeFiles/gl-test.dir/test/test.cpp.s

# Object files for target gl-test
gl__test_OBJECTS = \
"CMakeFiles/gl-test.dir/test/test.cpp.o"

# External object files for target gl-test
gl__test_EXTERNAL_OBJECTS =

gl-test: CMakeFiles/gl-test.dir/test/test.cpp.o
gl-test: CMakeFiles/gl-test.dir/build.make
gl-test: /usr/lib/x86_64-linux-gnu/libGL.so
gl-test: /usr/lib/x86_64-linux-gnu/libGLU.so
gl-test: /usr/lib/x86_64-linux-gnu/libglut.so
gl-test: CMakeFiles/gl-test.dir/link.txt
	@$(CMAKE_COMMAND) -E cmake_echo_color --switch=$(COLOR) --green --bold --progress-dir=/tmp/tmp.TJwn1KKsci/cmake-build-debug/CMakeFiles --progress-num=$(CMAKE_PROGRESS_2) "Linking CXX executable gl-test"
	$(CMAKE_COMMAND) -E cmake_link_script CMakeFiles/gl-test.dir/link.txt --verbose=$(VERBOSE)

# Rule to build all files generated by this target.
CMakeFiles/gl-test.dir/build: gl-test

.PHONY : CMakeFiles/gl-test.dir/build

CMakeFiles/gl-test.dir/clean:
	$(CMAKE_COMMAND) -P CMakeFiles/gl-test.dir/cmake_clean.cmake
.PHONY : CMakeFiles/gl-test.dir/clean

CMakeFiles/gl-test.dir/depend:
	cd /tmp/tmp.TJwn1KKsci/cmake-build-debug && $(CMAKE_COMMAND) -E cmake_depends "Unix Makefiles" /tmp/tmp.TJwn1KKsci /tmp/tmp.TJwn1KKsci /tmp/tmp.TJwn1KKsci/cmake-build-debug /tmp/tmp.TJwn1KKsci/cmake-build-debug /tmp/tmp.TJwn1KKsci/cmake-build-debug/CMakeFiles/gl-test.dir/DependInfo.cmake --color=$(COLOR)
.PHONY : CMakeFiles/gl-test.dir/depend

