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
CMAKE_SOURCE_DIR = /Users/yuchen/Nyu/graphics/Example

# The top-level build directory on which CMake was run.
CMAKE_BINARY_DIR = /Users/yuchen/Nyu/graphics/Example/build

# Include any dependencies generated for this target.
include CMakeFiles/Example.dir/depend.make

# Include the progress variables for this target.
include CMakeFiles/Example.dir/progress.make

# Include the compile flags for this target's objects.
include CMakeFiles/Example.dir/flags.make

CMakeFiles/Example.dir/example.cpp.o: CMakeFiles/Example.dir/flags.make
CMakeFiles/Example.dir/example.cpp.o: ../example.cpp
	@$(CMAKE_COMMAND) -E cmake_echo_color --switch=$(COLOR) --green --progress-dir=/Users/yuchen/Nyu/graphics/Example/build/CMakeFiles --progress-num=$(CMAKE_PROGRESS_1) "Building CXX object CMakeFiles/Example.dir/example.cpp.o"
	/Library/Developer/CommandLineTools/usr/bin/c++  $(CXX_DEFINES) $(CXX_INCLUDES) $(CXX_FLAGS) -o CMakeFiles/Example.dir/example.cpp.o -c /Users/yuchen/Nyu/graphics/Example/example.cpp

CMakeFiles/Example.dir/example.cpp.i: cmake_force
	@$(CMAKE_COMMAND) -E cmake_echo_color --switch=$(COLOR) --green "Preprocessing CXX source to CMakeFiles/Example.dir/example.cpp.i"
	/Library/Developer/CommandLineTools/usr/bin/c++ $(CXX_DEFINES) $(CXX_INCLUDES) $(CXX_FLAGS) -E /Users/yuchen/Nyu/graphics/Example/example.cpp > CMakeFiles/Example.dir/example.cpp.i

CMakeFiles/Example.dir/example.cpp.s: cmake_force
	@$(CMAKE_COMMAND) -E cmake_echo_color --switch=$(COLOR) --green "Compiling CXX source to assembly CMakeFiles/Example.dir/example.cpp.s"
	/Library/Developer/CommandLineTools/usr/bin/c++ $(CXX_DEFINES) $(CXX_INCLUDES) $(CXX_FLAGS) -S /Users/yuchen/Nyu/graphics/Example/example.cpp -o CMakeFiles/Example.dir/example.cpp.s

CMakeFiles/Example.dir/sample.cpp.o: CMakeFiles/Example.dir/flags.make
CMakeFiles/Example.dir/sample.cpp.o: ../sample.cpp
	@$(CMAKE_COMMAND) -E cmake_echo_color --switch=$(COLOR) --green --progress-dir=/Users/yuchen/Nyu/graphics/Example/build/CMakeFiles --progress-num=$(CMAKE_PROGRESS_2) "Building CXX object CMakeFiles/Example.dir/sample.cpp.o"
	/Library/Developer/CommandLineTools/usr/bin/c++  $(CXX_DEFINES) $(CXX_INCLUDES) $(CXX_FLAGS) -o CMakeFiles/Example.dir/sample.cpp.o -c /Users/yuchen/Nyu/graphics/Example/sample.cpp

CMakeFiles/Example.dir/sample.cpp.i: cmake_force
	@$(CMAKE_COMMAND) -E cmake_echo_color --switch=$(COLOR) --green "Preprocessing CXX source to CMakeFiles/Example.dir/sample.cpp.i"
	/Library/Developer/CommandLineTools/usr/bin/c++ $(CXX_DEFINES) $(CXX_INCLUDES) $(CXX_FLAGS) -E /Users/yuchen/Nyu/graphics/Example/sample.cpp > CMakeFiles/Example.dir/sample.cpp.i

CMakeFiles/Example.dir/sample.cpp.s: cmake_force
	@$(CMAKE_COMMAND) -E cmake_echo_color --switch=$(COLOR) --green "Compiling CXX source to assembly CMakeFiles/Example.dir/sample.cpp.s"
	/Library/Developer/CommandLineTools/usr/bin/c++ $(CXX_DEFINES) $(CXX_INCLUDES) $(CXX_FLAGS) -S /Users/yuchen/Nyu/graphics/Example/sample.cpp -o CMakeFiles/Example.dir/sample.cpp.s

# Object files for target Example
Example_OBJECTS = \
"CMakeFiles/Example.dir/example.cpp.o" \
"CMakeFiles/Example.dir/sample.cpp.o"

# External object files for target Example
Example_EXTERNAL_OBJECTS =

Example: CMakeFiles/Example.dir/example.cpp.o
Example: CMakeFiles/Example.dir/sample.cpp.o
Example: CMakeFiles/Example.dir/build.make
Example: CMakeFiles/Example.dir/link.txt
	@$(CMAKE_COMMAND) -E cmake_echo_color --switch=$(COLOR) --green --bold --progress-dir=/Users/yuchen/Nyu/graphics/Example/build/CMakeFiles --progress-num=$(CMAKE_PROGRESS_3) "Linking CXX executable Example"
	$(CMAKE_COMMAND) -E cmake_link_script CMakeFiles/Example.dir/link.txt --verbose=$(VERBOSE)

# Rule to build all files generated by this target.
CMakeFiles/Example.dir/build: Example

.PHONY : CMakeFiles/Example.dir/build

CMakeFiles/Example.dir/clean:
	$(CMAKE_COMMAND) -P CMakeFiles/Example.dir/cmake_clean.cmake
.PHONY : CMakeFiles/Example.dir/clean

CMakeFiles/Example.dir/depend:
	cd /Users/yuchen/Nyu/graphics/Example/build && $(CMAKE_COMMAND) -E cmake_depends "Unix Makefiles" /Users/yuchen/Nyu/graphics/Example /Users/yuchen/Nyu/graphics/Example /Users/yuchen/Nyu/graphics/Example/build /Users/yuchen/Nyu/graphics/Example/build /Users/yuchen/Nyu/graphics/Example/build/CMakeFiles/Example.dir/DependInfo.cmake --color=$(COLOR)
.PHONY : CMakeFiles/Example.dir/depend

