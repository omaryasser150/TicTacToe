# CMAKE generated file: DO NOT EDIT!
# Generated by "MinGW Makefiles" Generator, CMake Version 4.0

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

#Suppress display of executed commands.
$(VERBOSE).SILENT:

# A target that is always out of date.
cmake_force:
.PHONY : cmake_force

#=============================================================================
# Set environment variables for the build.

SHELL = cmd.exe

# The CMake executable.
CMAKE_COMMAND = C:\msys64\mingw64\bin\cmake.exe

# The command to remove a file.
RM = C:\msys64\mingw64\bin\cmake.exe -E rm -f

# Escaping for special characters.
EQUALS = =

# The top-level source directory on which CMake was run.
CMAKE_SOURCE_DIR = C:\Users\omary\My-Github\TicTacToe

# The top-level build directory on which CMake was run.
CMAKE_BINARY_DIR = C:\Users\omary\My-Github\TicTacToe\build

# Utility rule file for Documentation.

# Include any custom commands dependencies for this target.
include CMakeFiles/Documentation.dir/compiler_depend.make

# Include the progress variables for this target.
include CMakeFiles/Documentation.dir/progress.make

CMakeFiles/Documentation.dir/codegen:
.PHONY : CMakeFiles/Documentation.dir/codegen

Documentation: CMakeFiles/Documentation.dir/build.make
.PHONY : Documentation

# Rule to build all files generated by this target.
CMakeFiles/Documentation.dir/build: Documentation
.PHONY : CMakeFiles/Documentation.dir/build

CMakeFiles/Documentation.dir/clean:
	$(CMAKE_COMMAND) -P CMakeFiles\Documentation.dir\cmake_clean.cmake
.PHONY : CMakeFiles/Documentation.dir/clean

CMakeFiles/Documentation.dir/depend:
	$(CMAKE_COMMAND) -E cmake_depends "MinGW Makefiles" C:\Users\omary\My-Github\TicTacToe C:\Users\omary\My-Github\TicTacToe C:\Users\omary\My-Github\TicTacToe\build C:\Users\omary\My-Github\TicTacToe\build C:\Users\omary\My-Github\TicTacToe\build\CMakeFiles\Documentation.dir\DependInfo.cmake "--color=$(COLOR)"
.PHONY : CMakeFiles/Documentation.dir/depend

