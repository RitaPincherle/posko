# CMAKE generated file: DO NOT EDIT!
# Generated by "Unix Makefiles" Generator, CMake Version 3.28

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
CMAKE_SOURCE_DIR = /home/tarcakova/pokus

# The top-level build directory on which CMake was run.
CMAKE_BINARY_DIR = /home/tarcakova/pokus/build

# Include any dependencies generated for this target.
include shared/CMakeFiles/shared.dir/depend.make
# Include any dependencies generated by the compiler for this target.
include shared/CMakeFiles/shared.dir/compiler_depend.make

# Include the progress variables for this target.
include shared/CMakeFiles/shared.dir/progress.make

# Include the compile flags for this target's objects.
include shared/CMakeFiles/shared.dir/flags.make

shared/CMakeFiles/shared.dir/board.c.o: shared/CMakeFiles/shared.dir/flags.make
shared/CMakeFiles/shared.dir/board.c.o: /home/tarcakova/pokus/shared/board.c
shared/CMakeFiles/shared.dir/board.c.o: shared/CMakeFiles/shared.dir/compiler_depend.ts
	@$(CMAKE_COMMAND) -E cmake_echo_color "--switch=$(COLOR)" --green --progress-dir=/home/tarcakova/pokus/build/CMakeFiles --progress-num=$(CMAKE_PROGRESS_1) "Building C object shared/CMakeFiles/shared.dir/board.c.o"
	cd /home/tarcakova/pokus/build/shared && /usr/bin/cc $(C_DEFINES) $(C_INCLUDES) $(C_FLAGS) -MD -MT shared/CMakeFiles/shared.dir/board.c.o -MF CMakeFiles/shared.dir/board.c.o.d -o CMakeFiles/shared.dir/board.c.o -c /home/tarcakova/pokus/shared/board.c

shared/CMakeFiles/shared.dir/board.c.i: cmake_force
	@$(CMAKE_COMMAND) -E cmake_echo_color "--switch=$(COLOR)" --green "Preprocessing C source to CMakeFiles/shared.dir/board.c.i"
	cd /home/tarcakova/pokus/build/shared && /usr/bin/cc $(C_DEFINES) $(C_INCLUDES) $(C_FLAGS) -E /home/tarcakova/pokus/shared/board.c > CMakeFiles/shared.dir/board.c.i

shared/CMakeFiles/shared.dir/board.c.s: cmake_force
	@$(CMAKE_COMMAND) -E cmake_echo_color "--switch=$(COLOR)" --green "Compiling C source to assembly CMakeFiles/shared.dir/board.c.s"
	cd /home/tarcakova/pokus/build/shared && /usr/bin/cc $(C_DEFINES) $(C_INCLUDES) $(C_FLAGS) -S /home/tarcakova/pokus/shared/board.c -o CMakeFiles/shared.dir/board.c.s

shared/CMakeFiles/shared.dir/snake.c.o: shared/CMakeFiles/shared.dir/flags.make
shared/CMakeFiles/shared.dir/snake.c.o: /home/tarcakova/pokus/shared/snake.c
shared/CMakeFiles/shared.dir/snake.c.o: shared/CMakeFiles/shared.dir/compiler_depend.ts
	@$(CMAKE_COMMAND) -E cmake_echo_color "--switch=$(COLOR)" --green --progress-dir=/home/tarcakova/pokus/build/CMakeFiles --progress-num=$(CMAKE_PROGRESS_2) "Building C object shared/CMakeFiles/shared.dir/snake.c.o"
	cd /home/tarcakova/pokus/build/shared && /usr/bin/cc $(C_DEFINES) $(C_INCLUDES) $(C_FLAGS) -MD -MT shared/CMakeFiles/shared.dir/snake.c.o -MF CMakeFiles/shared.dir/snake.c.o.d -o CMakeFiles/shared.dir/snake.c.o -c /home/tarcakova/pokus/shared/snake.c

shared/CMakeFiles/shared.dir/snake.c.i: cmake_force
	@$(CMAKE_COMMAND) -E cmake_echo_color "--switch=$(COLOR)" --green "Preprocessing C source to CMakeFiles/shared.dir/snake.c.i"
	cd /home/tarcakova/pokus/build/shared && /usr/bin/cc $(C_DEFINES) $(C_INCLUDES) $(C_FLAGS) -E /home/tarcakova/pokus/shared/snake.c > CMakeFiles/shared.dir/snake.c.i

shared/CMakeFiles/shared.dir/snake.c.s: cmake_force
	@$(CMAKE_COMMAND) -E cmake_echo_color "--switch=$(COLOR)" --green "Compiling C source to assembly CMakeFiles/shared.dir/snake.c.s"
	cd /home/tarcakova/pokus/build/shared && /usr/bin/cc $(C_DEFINES) $(C_INCLUDES) $(C_FLAGS) -S /home/tarcakova/pokus/shared/snake.c -o CMakeFiles/shared.dir/snake.c.s

shared/CMakeFiles/shared.dir/timer.c.o: shared/CMakeFiles/shared.dir/flags.make
shared/CMakeFiles/shared.dir/timer.c.o: /home/tarcakova/pokus/shared/timer.c
shared/CMakeFiles/shared.dir/timer.c.o: shared/CMakeFiles/shared.dir/compiler_depend.ts
	@$(CMAKE_COMMAND) -E cmake_echo_color "--switch=$(COLOR)" --green --progress-dir=/home/tarcakova/pokus/build/CMakeFiles --progress-num=$(CMAKE_PROGRESS_3) "Building C object shared/CMakeFiles/shared.dir/timer.c.o"
	cd /home/tarcakova/pokus/build/shared && /usr/bin/cc $(C_DEFINES) $(C_INCLUDES) $(C_FLAGS) -MD -MT shared/CMakeFiles/shared.dir/timer.c.o -MF CMakeFiles/shared.dir/timer.c.o.d -o CMakeFiles/shared.dir/timer.c.o -c /home/tarcakova/pokus/shared/timer.c

shared/CMakeFiles/shared.dir/timer.c.i: cmake_force
	@$(CMAKE_COMMAND) -E cmake_echo_color "--switch=$(COLOR)" --green "Preprocessing C source to CMakeFiles/shared.dir/timer.c.i"
	cd /home/tarcakova/pokus/build/shared && /usr/bin/cc $(C_DEFINES) $(C_INCLUDES) $(C_FLAGS) -E /home/tarcakova/pokus/shared/timer.c > CMakeFiles/shared.dir/timer.c.i

shared/CMakeFiles/shared.dir/timer.c.s: cmake_force
	@$(CMAKE_COMMAND) -E cmake_echo_color "--switch=$(COLOR)" --green "Compiling C source to assembly CMakeFiles/shared.dir/timer.c.s"
	cd /home/tarcakova/pokus/build/shared && /usr/bin/cc $(C_DEFINES) $(C_INCLUDES) $(C_FLAGS) -S /home/tarcakova/pokus/shared/timer.c -o CMakeFiles/shared.dir/timer.c.s

# Object files for target shared
shared_OBJECTS = \
"CMakeFiles/shared.dir/board.c.o" \
"CMakeFiles/shared.dir/snake.c.o" \
"CMakeFiles/shared.dir/timer.c.o"

# External object files for target shared
shared_EXTERNAL_OBJECTS =

shared/libshared.a: shared/CMakeFiles/shared.dir/board.c.o
shared/libshared.a: shared/CMakeFiles/shared.dir/snake.c.o
shared/libshared.a: shared/CMakeFiles/shared.dir/timer.c.o
shared/libshared.a: shared/CMakeFiles/shared.dir/build.make
shared/libshared.a: shared/CMakeFiles/shared.dir/link.txt
	@$(CMAKE_COMMAND) -E cmake_echo_color "--switch=$(COLOR)" --green --bold --progress-dir=/home/tarcakova/pokus/build/CMakeFiles --progress-num=$(CMAKE_PROGRESS_4) "Linking C static library libshared.a"
	cd /home/tarcakova/pokus/build/shared && $(CMAKE_COMMAND) -P CMakeFiles/shared.dir/cmake_clean_target.cmake
	cd /home/tarcakova/pokus/build/shared && $(CMAKE_COMMAND) -E cmake_link_script CMakeFiles/shared.dir/link.txt --verbose=$(VERBOSE)

# Rule to build all files generated by this target.
shared/CMakeFiles/shared.dir/build: shared/libshared.a
.PHONY : shared/CMakeFiles/shared.dir/build

shared/CMakeFiles/shared.dir/clean:
	cd /home/tarcakova/pokus/build/shared && $(CMAKE_COMMAND) -P CMakeFiles/shared.dir/cmake_clean.cmake
.PHONY : shared/CMakeFiles/shared.dir/clean

shared/CMakeFiles/shared.dir/depend:
	cd /home/tarcakova/pokus/build && $(CMAKE_COMMAND) -E cmake_depends "Unix Makefiles" /home/tarcakova/pokus /home/tarcakova/pokus/shared /home/tarcakova/pokus/build /home/tarcakova/pokus/build/shared /home/tarcakova/pokus/build/shared/CMakeFiles/shared.dir/DependInfo.cmake "--color=$(COLOR)"
.PHONY : shared/CMakeFiles/shared.dir/depend

