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
CMAKE_SOURCE_DIR = /Users/yuchen/Nyu/graphics/hw1

# The top-level build directory on which CMake was run.
CMAKE_BINARY_DIR = /Users/yuchen/Nyu/graphics/hw1/build

# Include any dependencies generated for this target.
include CMakeFiles/Circle.dir/depend.make

# Include the progress variables for this target.
include CMakeFiles/Circle.dir/progress.make

# Include the compile flags for this target's objects.
include CMakeFiles/Circle.dir/flags.make

CMakeFiles/Circle.dir/draw_circle.cpp.o: CMakeFiles/Circle.dir/flags.make
CMakeFiles/Circle.dir/draw_circle.cpp.o: ../draw_circle.cpp
	@$(CMAKE_COMMAND) -E cmake_echo_color --switch=$(COLOR) --green --progress-dir=/Users/yuchen/Nyu/graphics/hw1/build/CMakeFiles --progress-num=$(CMAKE_PROGRESS_1) "Building CXX object CMakeFiles/Circle.dir/draw_circle.cpp.o"
	/Library/Developer/CommandLineTools/usr/bin/c++  $(CXX_DEFINES) $(CXX_INCLUDES) $(CXX_FLAGS) -o CMakeFiles/Circle.dir/draw_circle.cpp.o -c /Users/yuchen/Nyu/graphics/hw1/draw_circle.cpp

CMakeFiles/Circle.dir/draw_circle.cpp.i: cmake_force
	@$(CMAKE_COMMAND) -E cmake_echo_color --switch=$(COLOR) --green "Preprocessing CXX source to CMakeFiles/Circle.dir/draw_circle.cpp.i"
	/Library/Developer/CommandLineTools/usr/bin/c++ $(CXX_DEFINES) $(CXX_INCLUDES) $(CXX_FLAGS) -E /Users/yuchen/Nyu/graphics/hw1/draw_circle.cpp > CMakeFiles/Circle.dir/draw_circle.cpp.i

CMakeFiles/Circle.dir/draw_circle.cpp.s: cmake_force
	@$(CMAKE_COMMAND) -E cmake_echo_color --switch=$(COLOR) --green "Compiling CXX source to assembly CMakeFiles/Circle.dir/draw_circle.cpp.s"
	/Library/Developer/CommandLineTools/usr/bin/c++ $(CXX_DEFINES) $(CXX_INCLUDES) $(CXX_FLAGS) -S /Users/yuchen/Nyu/graphics/hw1/draw_circle.cpp -o CMakeFiles/Circle.dir/draw_circle.cpp.s

# Object files for target Circle
Circle_OBJECTS = \
"CMakeFiles/Circle.dir/draw_circle.cpp.o"

# External object files for target Circle
Circle_EXTERNAL_OBJECTS =

Circle: CMakeFiles/Circle.dir/draw_circle.cpp.o
Circle: CMakeFiles/Circle.dir/build.make
Circle: CMakeFiles/Circle.dir/link.txt
	@$(CMAKE_COMMAND) -E cmake_echo_color --switch=$(COLOR) --green --bold --progress-dir=/Users/yuchen/Nyu/graphics/hw1/build/CMakeFiles --progress-num=$(CMAKE_PROGRESS_2) "Linking CXX executable Circle"
	$(CMAKE_COMMAND) -E cmake_link_script CMakeFiles/Circle.dir/link.txt --verbose=$(VERBOSE)

# Rule to build all files generated by this target.
CMakeFiles/Circle.dir/build: Circle

.PHONY : CMakeFiles/Circle.dir/build

CMakeFiles/Circle.dir/clean:
	$(CMAKE_COMMAND) -P CMakeFiles/Circle.dir/cmake_clean.cmake
.PHONY : CMakeFiles/Circle.dir/clean

CMakeFiles/Circle.dir/depend:
	cd /Users/yuchen/Nyu/graphics/hw1/build && $(CMAKE_COMMAND) -E cmake_depends "Unix Makefiles" /Users/yuchen/Nyu/graphics/hw1 /Users/yuchen/Nyu/graphics/hw1 /Users/yuchen/Nyu/graphics/hw1/build /Users/yuchen/Nyu/graphics/hw1/build /Users/yuchen/Nyu/graphics/hw1/build/CMakeFiles/Circle.dir/DependInfo.cmake --color=$(COLOR)
.PHONY : CMakeFiles/Circle.dir/depend

