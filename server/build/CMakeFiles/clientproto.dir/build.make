# CMAKE generated file: DO NOT EDIT!
# Generated by "Unix Makefiles" Generator, CMake Version 3.10

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
CMAKE_SOURCE_DIR = /home/tiago/PARI/Projeto_77001/server

# The top-level build directory on which CMake was run.
CMAKE_BINARY_DIR = /home/tiago/PARI/Projeto_77001/server/build

# Utility rule file for clientproto.

# Include the progress variables for this target.
include CMakeFiles/clientproto.dir/progress.make

CMakeFiles/clientproto:
	cd /home/tiago/PARI/Projeto_77001/server && echo Rebuild\ prototypes.h
	cd /home/tiago/PARI/Projeto_77001/server && echo '/*File generated automatically. Do not edit*/' > /home/tiago/PARI/Projeto_77001/server/src/client/prototypes.h
	cd /home/tiago/PARI/Projeto_77001/server && echo '/*Generated on' `hostname` ',' `date` '*/' >> /home/tiago/PARI/Projeto_77001/server/src/client/prototypes.h
	cd /home/tiago/PARI/Projeto_77001/server && echo '#ifdef __cplusplus' >> /home/tiago/PARI/Projeto_77001/server/src/client/prototypes.h
	cd /home/tiago/PARI/Projeto_77001/server && echo 'extern "C" {' >> /home/tiago/PARI/Projeto_77001/server/src/client/prototypes.h
	cd /home/tiago/PARI/Projeto_77001/server && echo '#endif' >> /home/tiago/PARI/Projeto_77001/server/src/client/prototypes.h
	cd /home/tiago/PARI/Projeto_77001/server && cproto -q -O /dev/null >> /home/tiago/PARI/Projeto_77001/server/src/client/prototypes.h
	cd /home/tiago/PARI/Projeto_77001/server && echo '#ifdef __cplusplus' >> /home/tiago/PARI/Projeto_77001/server/src/client/prototypes.h
	cd /home/tiago/PARI/Projeto_77001/server && echo '}' >> /home/tiago/PARI/Projeto_77001/server/src/client/prototypes.h
	cd /home/tiago/PARI/Projeto_77001/server && echo '#endif' >> /home/tiago/PARI/Projeto_77001/server/src/client/prototypes.h

clientproto: CMakeFiles/clientproto
clientproto: CMakeFiles/clientproto.dir/build.make

.PHONY : clientproto

# Rule to build all files generated by this target.
CMakeFiles/clientproto.dir/build: clientproto

.PHONY : CMakeFiles/clientproto.dir/build

CMakeFiles/clientproto.dir/clean:
	$(CMAKE_COMMAND) -P CMakeFiles/clientproto.dir/cmake_clean.cmake
.PHONY : CMakeFiles/clientproto.dir/clean

CMakeFiles/clientproto.dir/depend:
	cd /home/tiago/PARI/Projeto_77001/server/build && $(CMAKE_COMMAND) -E cmake_depends "Unix Makefiles" /home/tiago/PARI/Projeto_77001/server /home/tiago/PARI/Projeto_77001/server /home/tiago/PARI/Projeto_77001/server/build /home/tiago/PARI/Projeto_77001/server/build /home/tiago/PARI/Projeto_77001/server/build/CMakeFiles/clientproto.dir/DependInfo.cmake --color=$(COLOR)
.PHONY : CMakeFiles/clientproto.dir/depend

