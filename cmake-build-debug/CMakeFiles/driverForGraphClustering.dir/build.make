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
include CMakeFiles/driverForGraphClustering.dir/depend.make

# Include the progress variables for this target.
include CMakeFiles/driverForGraphClustering.dir/progress.make

# Include the compile flags for this target's objects.
include CMakeFiles/driverForGraphClustering.dir/flags.make

CMakeFiles/driverForGraphClustering.dir/driverForGraphClustering.cpp.o: CMakeFiles/driverForGraphClustering.dir/flags.make
CMakeFiles/driverForGraphClustering.dir/driverForGraphClustering.cpp.o: ../driverForGraphClustering.cpp
	@$(CMAKE_COMMAND) -E cmake_echo_color --switch=$(COLOR) --green --progress-dir=/home/sriramsrinivas@unomaha.edu/CLionProjects/grappolo/cmake-build-debug/CMakeFiles --progress-num=$(CMAKE_PROGRESS_1) "Building CXX object CMakeFiles/driverForGraphClustering.dir/driverForGraphClustering.cpp.o"
	/usr/bin/c++  $(CXX_DEFINES) $(CXX_INCLUDES) $(CXX_FLAGS) -o CMakeFiles/driverForGraphClustering.dir/driverForGraphClustering.cpp.o -c /home/sriramsrinivas@unomaha.edu/CLionProjects/grappolo/driverForGraphClustering.cpp

CMakeFiles/driverForGraphClustering.dir/driverForGraphClustering.cpp.i: cmake_force
	@$(CMAKE_COMMAND) -E cmake_echo_color --switch=$(COLOR) --green "Preprocessing CXX source to CMakeFiles/driverForGraphClustering.dir/driverForGraphClustering.cpp.i"
	/usr/bin/c++ $(CXX_DEFINES) $(CXX_INCLUDES) $(CXX_FLAGS) -E /home/sriramsrinivas@unomaha.edu/CLionProjects/grappolo/driverForGraphClustering.cpp > CMakeFiles/driverForGraphClustering.dir/driverForGraphClustering.cpp.i

CMakeFiles/driverForGraphClustering.dir/driverForGraphClustering.cpp.s: cmake_force
	@$(CMAKE_COMMAND) -E cmake_echo_color --switch=$(COLOR) --green "Compiling CXX source to assembly CMakeFiles/driverForGraphClustering.dir/driverForGraphClustering.cpp.s"
	/usr/bin/c++ $(CXX_DEFINES) $(CXX_INCLUDES) $(CXX_FLAGS) -S /home/sriramsrinivas@unomaha.edu/CLionProjects/grappolo/driverForGraphClustering.cpp -o CMakeFiles/driverForGraphClustering.dir/driverForGraphClustering.cpp.s

# Object files for target driverForGraphClustering
driverForGraphClustering_OBJECTS = \
"CMakeFiles/driverForGraphClustering.dir/driverForGraphClustering.cpp.o"

# External object files for target driverForGraphClustering
driverForGraphClustering_EXTERNAL_OBJECTS =

driverForGraphClustering: CMakeFiles/driverForGraphClustering.dir/driverForGraphClustering.cpp.o
driverForGraphClustering: CMakeFiles/driverForGraphClustering.dir/build.make
driverForGraphClustering: InputsOutput/libinout.a
driverForGraphClustering: BasicCommunitiesDetection/libbasic_cd.a
driverForGraphClustering: Utility/libutil.a
driverForGraphClustering: FullSyncOptimization/libfull_syn_opt.a
driverForGraphClustering: Coloring/libcoloring.a
driverForGraphClustering: CMakeFiles/driverForGraphClustering.dir/link.txt
	@$(CMAKE_COMMAND) -E cmake_echo_color --switch=$(COLOR) --green --bold --progress-dir=/home/sriramsrinivas@unomaha.edu/CLionProjects/grappolo/cmake-build-debug/CMakeFiles --progress-num=$(CMAKE_PROGRESS_2) "Linking CXX executable driverForGraphClustering"
	$(CMAKE_COMMAND) -E cmake_link_script CMakeFiles/driverForGraphClustering.dir/link.txt --verbose=$(VERBOSE)

# Rule to build all files generated by this target.
CMakeFiles/driverForGraphClustering.dir/build: driverForGraphClustering

.PHONY : CMakeFiles/driverForGraphClustering.dir/build

CMakeFiles/driverForGraphClustering.dir/clean:
	$(CMAKE_COMMAND) -P CMakeFiles/driverForGraphClustering.dir/cmake_clean.cmake
.PHONY : CMakeFiles/driverForGraphClustering.dir/clean

CMakeFiles/driverForGraphClustering.dir/depend:
	cd /home/sriramsrinivas@unomaha.edu/CLionProjects/grappolo/cmake-build-debug && $(CMAKE_COMMAND) -E cmake_depends "Unix Makefiles" /home/sriramsrinivas@unomaha.edu/CLionProjects/grappolo /home/sriramsrinivas@unomaha.edu/CLionProjects/grappolo /home/sriramsrinivas@unomaha.edu/CLionProjects/grappolo/cmake-build-debug /home/sriramsrinivas@unomaha.edu/CLionProjects/grappolo/cmake-build-debug /home/sriramsrinivas@unomaha.edu/CLionProjects/grappolo/cmake-build-debug/CMakeFiles/driverForGraphClustering.dir/DependInfo.cmake --color=$(COLOR)
.PHONY : CMakeFiles/driverForGraphClustering.dir/depend

