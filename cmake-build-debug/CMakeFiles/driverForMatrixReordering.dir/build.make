# CMAKE generated file: DO NOT EDIT!
# Generated by "Unix Makefiles" Generator, CMake Version 3.15

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
CMAKE_COMMAND = /home/sriramsrinivas@unomaha.edu/Downloads/clion-2019.3.2/bin/cmake/linux/bin/cmake

# The command to remove a file.
RM = /home/sriramsrinivas@unomaha.edu/Downloads/clion-2019.3.2/bin/cmake/linux/bin/cmake -E remove -f

# Escaping for special characters.
EQUALS = =

# The top-level source directory on which CMake was run.
CMAKE_SOURCE_DIR = /home/sriramsrinivas@unomaha.edu/CLionProjects/grappolo

# The top-level build directory on which CMake was run.
CMAKE_BINARY_DIR = /home/sriramsrinivas@unomaha.edu/CLionProjects/grappolo/cmake-build-debug

# Include any dependencies generated for this target.
include CMakeFiles/driverForMatrixReordering.dir/depend.make

# Include the progress variables for this target.
include CMakeFiles/driverForMatrixReordering.dir/progress.make

# Include the compile flags for this target's objects.
include CMakeFiles/driverForMatrixReordering.dir/flags.make

CMakeFiles/driverForMatrixReordering.dir/driverForMatrixReordering.cpp.o: CMakeFiles/driverForMatrixReordering.dir/flags.make
CMakeFiles/driverForMatrixReordering.dir/driverForMatrixReordering.cpp.o: ../driverForMatrixReordering.cpp
	@$(CMAKE_COMMAND) -E cmake_echo_color --switch=$(COLOR) --green --progress-dir=/home/sriramsrinivas@unomaha.edu/CLionProjects/grappolo/cmake-build-debug/CMakeFiles --progress-num=$(CMAKE_PROGRESS_1) "Building CXX object CMakeFiles/driverForMatrixReordering.dir/driverForMatrixReordering.cpp.o"
	/usr/bin/c++  $(CXX_DEFINES) $(CXX_INCLUDES) $(CXX_FLAGS) -o CMakeFiles/driverForMatrixReordering.dir/driverForMatrixReordering.cpp.o -c /home/sriramsrinivas@unomaha.edu/CLionProjects/grappolo/driverForMatrixReordering.cpp

CMakeFiles/driverForMatrixReordering.dir/driverForMatrixReordering.cpp.i: cmake_force
	@$(CMAKE_COMMAND) -E cmake_echo_color --switch=$(COLOR) --green "Preprocessing CXX source to CMakeFiles/driverForMatrixReordering.dir/driverForMatrixReordering.cpp.i"
	/usr/bin/c++ $(CXX_DEFINES) $(CXX_INCLUDES) $(CXX_FLAGS) -E /home/sriramsrinivas@unomaha.edu/CLionProjects/grappolo/driverForMatrixReordering.cpp > CMakeFiles/driverForMatrixReordering.dir/driverForMatrixReordering.cpp.i

CMakeFiles/driverForMatrixReordering.dir/driverForMatrixReordering.cpp.s: cmake_force
	@$(CMAKE_COMMAND) -E cmake_echo_color --switch=$(COLOR) --green "Compiling CXX source to assembly CMakeFiles/driverForMatrixReordering.dir/driverForMatrixReordering.cpp.s"
	/usr/bin/c++ $(CXX_DEFINES) $(CXX_INCLUDES) $(CXX_FLAGS) -S /home/sriramsrinivas@unomaha.edu/CLionProjects/grappolo/driverForMatrixReordering.cpp -o CMakeFiles/driverForMatrixReordering.dir/driverForMatrixReordering.cpp.s

# Object files for target driverForMatrixReordering
driverForMatrixReordering_OBJECTS = \
"CMakeFiles/driverForMatrixReordering.dir/driverForMatrixReordering.cpp.o"

# External object files for target driverForMatrixReordering
driverForMatrixReordering_EXTERNAL_OBJECTS =

driverForMatrixReordering: CMakeFiles/driverForMatrixReordering.dir/driverForMatrixReordering.cpp.o
driverForMatrixReordering: CMakeFiles/driverForMatrixReordering.dir/build.make
driverForMatrixReordering: InputsOutput/libinout.a
driverForMatrixReordering: BasicCommunitiesDetection/libbasic_cd.a
driverForMatrixReordering: Utility/libutil.a
driverForMatrixReordering: FullSyncOptimization/libfull_syn_opt.a
driverForMatrixReordering: Coloring/libcoloring.a
driverForMatrixReordering: CMakeFiles/driverForMatrixReordering.dir/link.txt
	@$(CMAKE_COMMAND) -E cmake_echo_color --switch=$(COLOR) --green --bold --progress-dir=/home/sriramsrinivas@unomaha.edu/CLionProjects/grappolo/cmake-build-debug/CMakeFiles --progress-num=$(CMAKE_PROGRESS_2) "Linking CXX executable driverForMatrixReordering"
	$(CMAKE_COMMAND) -E cmake_link_script CMakeFiles/driverForMatrixReordering.dir/link.txt --verbose=$(VERBOSE)

# Rule to build all files generated by this target.
CMakeFiles/driverForMatrixReordering.dir/build: driverForMatrixReordering

.PHONY : CMakeFiles/driverForMatrixReordering.dir/build

CMakeFiles/driverForMatrixReordering.dir/clean:
	$(CMAKE_COMMAND) -P CMakeFiles/driverForMatrixReordering.dir/cmake_clean.cmake
.PHONY : CMakeFiles/driverForMatrixReordering.dir/clean

CMakeFiles/driverForMatrixReordering.dir/depend:
	cd /home/sriramsrinivas@unomaha.edu/CLionProjects/grappolo/cmake-build-debug && $(CMAKE_COMMAND) -E cmake_depends "Unix Makefiles" /home/sriramsrinivas@unomaha.edu/CLionProjects/grappolo /home/sriramsrinivas@unomaha.edu/CLionProjects/grappolo /home/sriramsrinivas@unomaha.edu/CLionProjects/grappolo/cmake-build-debug /home/sriramsrinivas@unomaha.edu/CLionProjects/grappolo/cmake-build-debug /home/sriramsrinivas@unomaha.edu/CLionProjects/grappolo/cmake-build-debug/CMakeFiles/driverForMatrixReordering.dir/DependInfo.cmake --color=$(COLOR)
.PHONY : CMakeFiles/driverForMatrixReordering.dir/depend

