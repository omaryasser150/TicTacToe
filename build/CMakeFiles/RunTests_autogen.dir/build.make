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

# Utility rule file for RunTests_autogen.

# Include any custom commands dependencies for this target.
include CMakeFiles/RunTests_autogen.dir/compiler_depend.make

# Include the progress variables for this target.
include CMakeFiles/RunTests_autogen.dir/progress.make

CMakeFiles/RunTests_autogen: RunTests_autogen/timestamp

RunTests_autogen/timestamp: C:/msys64/mingw64/share/qt6/bin/moc.exe
RunTests_autogen/timestamp: C:/msys64/mingw64/share/qt6/bin/uic.exe
RunTests_autogen/timestamp: CMakeFiles/RunTests_autogen.dir/compiler_depend.ts
	@$(CMAKE_COMMAND) -E cmake_echo_color "--switch=$(COLOR)" --blue --bold --progress-dir=C:\Users\omary\My-Github\TicTacToe\build\CMakeFiles --progress-num=$(CMAKE_PROGRESS_1) "Automatic MOC and UIC for target RunTests"
	C:\msys64\mingw64\bin\cmake.exe -E cmake_autogen C:/Users/omary/My-Github/TicTacToe/build/CMakeFiles/RunTests_autogen.dir/AutogenInfo.json ""
	C:\msys64\mingw64\bin\cmake.exe -E touch C:/Users/omary/My-Github/TicTacToe/build/RunTests_autogen/timestamp

CMakeFiles/RunTests_autogen.dir/codegen:
.PHONY : CMakeFiles/RunTests_autogen.dir/codegen

RunTests_autogen: CMakeFiles/RunTests_autogen
RunTests_autogen: RunTests_autogen/timestamp
RunTests_autogen: CMakeFiles/RunTests_autogen.dir/build.make
.PHONY : RunTests_autogen

# Rule to build all files generated by this target.
CMakeFiles/RunTests_autogen.dir/build: RunTests_autogen
.PHONY : CMakeFiles/RunTests_autogen.dir/build

CMakeFiles/RunTests_autogen.dir/clean:
	$(CMAKE_COMMAND) -P CMakeFiles\RunTests_autogen.dir\cmake_clean.cmake
.PHONY : CMakeFiles/RunTests_autogen.dir/clean

CMakeFiles/RunTests_autogen.dir/depend:
	$(CMAKE_COMMAND) -E cmake_depends "MinGW Makefiles" C:\Users\omary\My-Github\TicTacToe C:\Users\omary\My-Github\TicTacToe C:\Users\omary\My-Github\TicTacToe\build C:\Users\omary\My-Github\TicTacToe\build C:\Users\omary\My-Github\TicTacToe\build\CMakeFiles\RunTests_autogen.dir\DependInfo.cmake "--color=$(COLOR)"
.PHONY : CMakeFiles/RunTests_autogen.dir/depend

