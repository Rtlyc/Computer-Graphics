# CMAKE generated file: DO NOT EDIT!
# Generated by "Unix Makefiles" Generator, CMake Version 3.17

# Delete rule output on recipe failure.
.DELETE_ON_ERROR:


#=============================================================================
# Special targets provided by cmake.

# Disable implicit rules so canonical targets will work.
.SUFFIXES:


# Disable VCS-based implicit rules.
% : %,v


# Disable VCS-based implicit rules.
% : RCS/%


# Disable VCS-based implicit rules.
% : RCS/%,v


# Disable VCS-based implicit rules.
% : SCCS/s.%


# Disable VCS-based implicit rules.
% : s.%


.SUFFIXES: .hpux_make_needs_suffix_list


# Command-line flag to silence nested $(MAKE).
$(VERBOSE)MAKESILENT = -s

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
CMAKE_COMMAND = /usr/local/Cellar/cmake/3.17.1/bin/cmake

# The command to remove a file.
RM = /usr/local/Cellar/cmake/3.17.1/bin/cmake -E rm -f

# Escaping for special characters.
EQUALS = =

# The top-level source directory on which CMake was run.
CMAKE_SOURCE_DIR = /Users/yuchen/Nyu/graphics/hw2

# The top-level build directory on which CMake was run.
CMAKE_BINARY_DIR = /Users/yuchen/Nyu/graphics/hw2/build

# Include any dependencies generated for this target.
include CMakeFiles/RotateBall.dir/depend.make

# Include the progress variables for this target.
include CMakeFiles/RotateBall.dir/progress.make

# Include the compile flags for this target's objects.
include CMakeFiles/RotateBall.dir/flags.make

CMakeFiles/RotateBall.dir/InitShader.cpp.o: CMakeFiles/RotateBall.dir/flags.make
CMakeFiles/RotateBall.dir/InitShader.cpp.o: ../InitShader.cpp
	@$(CMAKE_COMMAND) -E cmake_echo_color --switch=$(COLOR) --green --progress-dir=/Users/yuchen/Nyu/graphics/hw2/build/CMakeFiles --progress-num=$(CMAKE_PROGRESS_1) "Building CXX object CMakeFiles/RotateBall.dir/InitShader.cpp.o"
	/Library/Developer/CommandLineTools/usr/bin/c++  $(CXX_DEFINES) $(CXX_INCLUDES) $(CXX_FLAGS) -o CMakeFiles/RotateBall.dir/InitShader.cpp.o -c /Users/yuchen/Nyu/graphics/hw2/InitShader.cpp

CMakeFiles/RotateBall.dir/InitShader.cpp.i: cmake_force
	@$(CMAKE_COMMAND) -E cmake_echo_color --switch=$(COLOR) --green "Preprocessing CXX source to CMakeFiles/RotateBall.dir/InitShader.cpp.i"
	/Library/Developer/CommandLineTools/usr/bin/c++ $(CXX_DEFINES) $(CXX_INCLUDES) $(CXX_FLAGS) -E /Users/yuchen/Nyu/graphics/hw2/InitShader.cpp > CMakeFiles/RotateBall.dir/InitShader.cpp.i

CMakeFiles/RotateBall.dir/InitShader.cpp.s: cmake_force
	@$(CMAKE_COMMAND) -E cmake_echo_color --switch=$(COLOR) --green "Compiling CXX source to assembly CMakeFiles/RotateBall.dir/InitShader.cpp.s"
	/Library/Developer/CommandLineTools/usr/bin/c++ $(CXX_DEFINES) $(CXX_INCLUDES) $(CXX_FLAGS) -S /Users/yuchen/Nyu/graphics/hw2/InitShader.cpp -o CMakeFiles/RotateBall.dir/InitShader.cpp.s

CMakeFiles/RotateBall.dir/rotate-ball.cpp.o: CMakeFiles/RotateBall.dir/flags.make
CMakeFiles/RotateBall.dir/rotate-ball.cpp.o: ../rotate-ball.cpp
	@$(CMAKE_COMMAND) -E cmake_echo_color --switch=$(COLOR) --green --progress-dir=/Users/yuchen/Nyu/graphics/hw2/build/CMakeFiles --progress-num=$(CMAKE_PROGRESS_2) "Building CXX object CMakeFiles/RotateBall.dir/rotate-ball.cpp.o"
	/Library/Developer/CommandLineTools/usr/bin/c++  $(CXX_DEFINES) $(CXX_INCLUDES) $(CXX_FLAGS) -o CMakeFiles/RotateBall.dir/rotate-ball.cpp.o -c /Users/yuchen/Nyu/graphics/hw2/rotate-ball.cpp

CMakeFiles/RotateBall.dir/rotate-ball.cpp.i: cmake_force
	@$(CMAKE_COMMAND) -E cmake_echo_color --switch=$(COLOR) --green "Preprocessing CXX source to CMakeFiles/RotateBall.dir/rotate-ball.cpp.i"
	/Library/Developer/CommandLineTools/usr/bin/c++ $(CXX_DEFINES) $(CXX_INCLUDES) $(CXX_FLAGS) -E /Users/yuchen/Nyu/graphics/hw2/rotate-ball.cpp > CMakeFiles/RotateBall.dir/rotate-ball.cpp.i

CMakeFiles/RotateBall.dir/rotate-ball.cpp.s: cmake_force
	@$(CMAKE_COMMAND) -E cmake_echo_color --switch=$(COLOR) --green "Compiling CXX source to assembly CMakeFiles/RotateBall.dir/rotate-ball.cpp.s"
	/Library/Developer/CommandLineTools/usr/bin/c++ $(CXX_DEFINES) $(CXX_INCLUDES) $(CXX_FLAGS) -S /Users/yuchen/Nyu/graphics/hw2/rotate-ball.cpp -o CMakeFiles/RotateBall.dir/rotate-ball.cpp.s

# Object files for target RotateBall
RotateBall_OBJECTS = \
"CMakeFiles/RotateBall.dir/InitShader.cpp.o" \
"CMakeFiles/RotateBall.dir/rotate-ball.cpp.o"

# External object files for target RotateBall
RotateBall_EXTERNAL_OBJECTS =

RotateBall: CMakeFiles/RotateBall.dir/InitShader.cpp.o
RotateBall: CMakeFiles/RotateBall.dir/rotate-ball.cpp.o
RotateBall: CMakeFiles/RotateBall.dir/build.make
RotateBall: CMakeFiles/RotateBall.dir/link.txt
	@$(CMAKE_COMMAND) -E cmake_echo_color --switch=$(COLOR) --green --bold --progress-dir=/Users/yuchen/Nyu/graphics/hw2/build/CMakeFiles --progress-num=$(CMAKE_PROGRESS_3) "Linking CXX executable RotateBall"
	$(CMAKE_COMMAND) -E cmake_link_script CMakeFiles/RotateBall.dir/link.txt --verbose=$(VERBOSE)

# Rule to build all files generated by this target.
CMakeFiles/RotateBall.dir/build: RotateBall

.PHONY : CMakeFiles/RotateBall.dir/build

CMakeFiles/RotateBall.dir/clean:
	$(CMAKE_COMMAND) -P CMakeFiles/RotateBall.dir/cmake_clean.cmake
.PHONY : CMakeFiles/RotateBall.dir/clean

CMakeFiles/RotateBall.dir/depend:
	cd /Users/yuchen/Nyu/graphics/hw2/build && $(CMAKE_COMMAND) -E cmake_depends "Unix Makefiles" /Users/yuchen/Nyu/graphics/hw2 /Users/yuchen/Nyu/graphics/hw2 /Users/yuchen/Nyu/graphics/hw2/build /Users/yuchen/Nyu/graphics/hw2/build /Users/yuchen/Nyu/graphics/hw2/build/CMakeFiles/RotateBall.dir/DependInfo.cmake --color=$(COLOR)
.PHONY : CMakeFiles/RotateBall.dir/depend

