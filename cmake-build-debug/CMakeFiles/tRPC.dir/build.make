# CMAKE generated file: DO NOT EDIT!
# Generated by "Unix Makefiles" Generator, CMake Version 3.22

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

# The shell in which to execute make rules.
SHELL = /bin/sh

# The CMake executable.
CMAKE_COMMAND = /usr/bin/cmake

# The command to remove a file.
RM = /usr/bin/cmake -E rm -f

# Escaping for special characters.
EQUALS = =

# The top-level source directory on which CMake was run.
CMAKE_SOURCE_DIR = /mnt/文档/cpp/project/tinyrpc

# The top-level build directory on which CMake was run.
CMAKE_BINARY_DIR = /mnt/文档/cpp/project/tinyrpc/cmake-build-debug

# Include any dependencies generated for this target.
include CMakeFiles/tRPC.dir/depend.make
# Include any dependencies generated by the compiler for this target.
include CMakeFiles/tRPC.dir/compiler_depend.make

# Include the progress variables for this target.
include CMakeFiles/tRPC.dir/progress.make

# Include the compile flags for this target's objects.
include CMakeFiles/tRPC.dir/flags.make

CMakeFiles/tRPC.dir/main.cpp.o: CMakeFiles/tRPC.dir/flags.make
CMakeFiles/tRPC.dir/main.cpp.o: ../main.cpp
CMakeFiles/tRPC.dir/main.cpp.o: CMakeFiles/tRPC.dir/compiler_depend.ts
	@$(CMAKE_COMMAND) -E cmake_echo_color --switch=$(COLOR) --green --progress-dir=/mnt/文档/cpp/project/tinyrpc/cmake-build-debug/CMakeFiles --progress-num=$(CMAKE_PROGRESS_1) "Building CXX object CMakeFiles/tRPC.dir/main.cpp.o"
	/usr/bin/c++ $(CXX_DEFINES) $(CXX_INCLUDES) $(CXX_FLAGS) -MD -MT CMakeFiles/tRPC.dir/main.cpp.o -MF CMakeFiles/tRPC.dir/main.cpp.o.d -o CMakeFiles/tRPC.dir/main.cpp.o -c /mnt/文档/cpp/project/tinyrpc/main.cpp

CMakeFiles/tRPC.dir/main.cpp.i: cmake_force
	@$(CMAKE_COMMAND) -E cmake_echo_color --switch=$(COLOR) --green "Preprocessing CXX source to CMakeFiles/tRPC.dir/main.cpp.i"
	/usr/bin/c++ $(CXX_DEFINES) $(CXX_INCLUDES) $(CXX_FLAGS) -E /mnt/文档/cpp/project/tinyrpc/main.cpp > CMakeFiles/tRPC.dir/main.cpp.i

CMakeFiles/tRPC.dir/main.cpp.s: cmake_force
	@$(CMAKE_COMMAND) -E cmake_echo_color --switch=$(COLOR) --green "Compiling CXX source to assembly CMakeFiles/tRPC.dir/main.cpp.s"
	/usr/bin/c++ $(CXX_DEFINES) $(CXX_INCLUDES) $(CXX_FLAGS) -S /mnt/文档/cpp/project/tinyrpc/main.cpp -o CMakeFiles/tRPC.dir/main.cpp.s

# Object files for target tRPC
tRPC_OBJECTS = \
"CMakeFiles/tRPC.dir/main.cpp.o"

# External object files for target tRPC
tRPC_EXTERNAL_OBJECTS =

tRPC: CMakeFiles/tRPC.dir/main.cpp.o
tRPC: CMakeFiles/tRPC.dir/build.make
tRPC: CMakeFiles/tRPC.dir/link.txt
	@$(CMAKE_COMMAND) -E cmake_echo_color --switch=$(COLOR) --green --bold --progress-dir=/mnt/文档/cpp/project/tinyrpc/cmake-build-debug/CMakeFiles --progress-num=$(CMAKE_PROGRESS_2) "Linking CXX executable tRPC"
	$(CMAKE_COMMAND) -E cmake_link_script CMakeFiles/tRPC.dir/link.txt --verbose=$(VERBOSE)

# Rule to build all files generated by this target.
CMakeFiles/tRPC.dir/build: tRPC
.PHONY : CMakeFiles/tRPC.dir/build

CMakeFiles/tRPC.dir/clean:
	$(CMAKE_COMMAND) -P CMakeFiles/tRPC.dir/cmake_clean.cmake
.PHONY : CMakeFiles/tRPC.dir/clean

CMakeFiles/tRPC.dir/depend:
	cd /mnt/文档/cpp/project/tinyrpc/cmake-build-debug && $(CMAKE_COMMAND) -E cmake_depends "Unix Makefiles" /mnt/文档/cpp/project/tinyrpc /mnt/文档/cpp/project/tinyrpc /mnt/文档/cpp/project/tinyrpc/cmake-build-debug /mnt/文档/cpp/project/tinyrpc/cmake-build-debug /mnt/文档/cpp/project/tinyrpc/cmake-build-debug/CMakeFiles/tRPC.dir/DependInfo.cmake --color=$(COLOR)
.PHONY : CMakeFiles/tRPC.dir/depend
