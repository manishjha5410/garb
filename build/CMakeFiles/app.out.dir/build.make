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
CMAKE_SOURCE_DIR = /home/manish/Music/Dolat-Hackathon

# The top-level build directory on which CMake was run.
CMAKE_BINARY_DIR = /home/manish/Music/Dolat-Hackathon/build

# Include any dependencies generated for this target.
include CMakeFiles/app.out.dir/depend.make
# Include any dependencies generated by the compiler for this target.
include CMakeFiles/app.out.dir/compiler_depend.make

# Include the progress variables for this target.
include CMakeFiles/app.out.dir/progress.make

# Include the compile flags for this target's objects.
include CMakeFiles/app.out.dir/flags.make

CMakeFiles/app.out.dir/BACKEND/server.cpp.o: CMakeFiles/app.out.dir/flags.make
CMakeFiles/app.out.dir/BACKEND/server.cpp.o: ../BACKEND/server.cpp
CMakeFiles/app.out.dir/BACKEND/server.cpp.o: CMakeFiles/app.out.dir/compiler_depend.ts
	@$(CMAKE_COMMAND) -E cmake_echo_color --switch=$(COLOR) --green --progress-dir=/home/manish/Music/Dolat-Hackathon/build/CMakeFiles --progress-num=$(CMAKE_PROGRESS_1) "Building CXX object CMakeFiles/app.out.dir/BACKEND/server.cpp.o"
	/usr/bin/g++ $(CXX_DEFINES) $(CXX_INCLUDES) $(CXX_FLAGS) -MD -MT CMakeFiles/app.out.dir/BACKEND/server.cpp.o -MF CMakeFiles/app.out.dir/BACKEND/server.cpp.o.d -o CMakeFiles/app.out.dir/BACKEND/server.cpp.o -c /home/manish/Music/Dolat-Hackathon/BACKEND/server.cpp

CMakeFiles/app.out.dir/BACKEND/server.cpp.i: cmake_force
	@$(CMAKE_COMMAND) -E cmake_echo_color --switch=$(COLOR) --green "Preprocessing CXX source to CMakeFiles/app.out.dir/BACKEND/server.cpp.i"
	/usr/bin/g++ $(CXX_DEFINES) $(CXX_INCLUDES) $(CXX_FLAGS) -E /home/manish/Music/Dolat-Hackathon/BACKEND/server.cpp > CMakeFiles/app.out.dir/BACKEND/server.cpp.i

CMakeFiles/app.out.dir/BACKEND/server.cpp.s: cmake_force
	@$(CMAKE_COMMAND) -E cmake_echo_color --switch=$(COLOR) --green "Compiling CXX source to assembly CMakeFiles/app.out.dir/BACKEND/server.cpp.s"
	/usr/bin/g++ $(CXX_DEFINES) $(CXX_INCLUDES) $(CXX_FLAGS) -S /home/manish/Music/Dolat-Hackathon/BACKEND/server.cpp -o CMakeFiles/app.out.dir/BACKEND/server.cpp.s

CMakeFiles/app.out.dir/BACKEND/Module/Billing/Billing.cpp.o: CMakeFiles/app.out.dir/flags.make
CMakeFiles/app.out.dir/BACKEND/Module/Billing/Billing.cpp.o: ../BACKEND/Module/Billing/Billing.cpp
CMakeFiles/app.out.dir/BACKEND/Module/Billing/Billing.cpp.o: CMakeFiles/app.out.dir/compiler_depend.ts
	@$(CMAKE_COMMAND) -E cmake_echo_color --switch=$(COLOR) --green --progress-dir=/home/manish/Music/Dolat-Hackathon/build/CMakeFiles --progress-num=$(CMAKE_PROGRESS_2) "Building CXX object CMakeFiles/app.out.dir/BACKEND/Module/Billing/Billing.cpp.o"
	/usr/bin/g++ $(CXX_DEFINES) $(CXX_INCLUDES) $(CXX_FLAGS) -MD -MT CMakeFiles/app.out.dir/BACKEND/Module/Billing/Billing.cpp.o -MF CMakeFiles/app.out.dir/BACKEND/Module/Billing/Billing.cpp.o.d -o CMakeFiles/app.out.dir/BACKEND/Module/Billing/Billing.cpp.o -c /home/manish/Music/Dolat-Hackathon/BACKEND/Module/Billing/Billing.cpp

CMakeFiles/app.out.dir/BACKEND/Module/Billing/Billing.cpp.i: cmake_force
	@$(CMAKE_COMMAND) -E cmake_echo_color --switch=$(COLOR) --green "Preprocessing CXX source to CMakeFiles/app.out.dir/BACKEND/Module/Billing/Billing.cpp.i"
	/usr/bin/g++ $(CXX_DEFINES) $(CXX_INCLUDES) $(CXX_FLAGS) -E /home/manish/Music/Dolat-Hackathon/BACKEND/Module/Billing/Billing.cpp > CMakeFiles/app.out.dir/BACKEND/Module/Billing/Billing.cpp.i

CMakeFiles/app.out.dir/BACKEND/Module/Billing/Billing.cpp.s: cmake_force
	@$(CMAKE_COMMAND) -E cmake_echo_color --switch=$(COLOR) --green "Compiling CXX source to assembly CMakeFiles/app.out.dir/BACKEND/Module/Billing/Billing.cpp.s"
	/usr/bin/g++ $(CXX_DEFINES) $(CXX_INCLUDES) $(CXX_FLAGS) -S /home/manish/Music/Dolat-Hackathon/BACKEND/Module/Billing/Billing.cpp -o CMakeFiles/app.out.dir/BACKEND/Module/Billing/Billing.cpp.s

CMakeFiles/app.out.dir/BACKEND/Module/Request/RequestManagement.cpp.o: CMakeFiles/app.out.dir/flags.make
CMakeFiles/app.out.dir/BACKEND/Module/Request/RequestManagement.cpp.o: ../BACKEND/Module/Request/RequestManagement.cpp
CMakeFiles/app.out.dir/BACKEND/Module/Request/RequestManagement.cpp.o: CMakeFiles/app.out.dir/compiler_depend.ts
	@$(CMAKE_COMMAND) -E cmake_echo_color --switch=$(COLOR) --green --progress-dir=/home/manish/Music/Dolat-Hackathon/build/CMakeFiles --progress-num=$(CMAKE_PROGRESS_3) "Building CXX object CMakeFiles/app.out.dir/BACKEND/Module/Request/RequestManagement.cpp.o"
	/usr/bin/g++ $(CXX_DEFINES) $(CXX_INCLUDES) $(CXX_FLAGS) -MD -MT CMakeFiles/app.out.dir/BACKEND/Module/Request/RequestManagement.cpp.o -MF CMakeFiles/app.out.dir/BACKEND/Module/Request/RequestManagement.cpp.o.d -o CMakeFiles/app.out.dir/BACKEND/Module/Request/RequestManagement.cpp.o -c /home/manish/Music/Dolat-Hackathon/BACKEND/Module/Request/RequestManagement.cpp

CMakeFiles/app.out.dir/BACKEND/Module/Request/RequestManagement.cpp.i: cmake_force
	@$(CMAKE_COMMAND) -E cmake_echo_color --switch=$(COLOR) --green "Preprocessing CXX source to CMakeFiles/app.out.dir/BACKEND/Module/Request/RequestManagement.cpp.i"
	/usr/bin/g++ $(CXX_DEFINES) $(CXX_INCLUDES) $(CXX_FLAGS) -E /home/manish/Music/Dolat-Hackathon/BACKEND/Module/Request/RequestManagement.cpp > CMakeFiles/app.out.dir/BACKEND/Module/Request/RequestManagement.cpp.i

CMakeFiles/app.out.dir/BACKEND/Module/Request/RequestManagement.cpp.s: cmake_force
	@$(CMAKE_COMMAND) -E cmake_echo_color --switch=$(COLOR) --green "Compiling CXX source to assembly CMakeFiles/app.out.dir/BACKEND/Module/Request/RequestManagement.cpp.s"
	/usr/bin/g++ $(CXX_DEFINES) $(CXX_INCLUDES) $(CXX_FLAGS) -S /home/manish/Music/Dolat-Hackathon/BACKEND/Module/Request/RequestManagement.cpp -o CMakeFiles/app.out.dir/BACKEND/Module/Request/RequestManagement.cpp.s

CMakeFiles/app.out.dir/BACKEND/Module/Inventory/Inventory.cpp.o: CMakeFiles/app.out.dir/flags.make
CMakeFiles/app.out.dir/BACKEND/Module/Inventory/Inventory.cpp.o: ../BACKEND/Module/Inventory/Inventory.cpp
CMakeFiles/app.out.dir/BACKEND/Module/Inventory/Inventory.cpp.o: CMakeFiles/app.out.dir/compiler_depend.ts
	@$(CMAKE_COMMAND) -E cmake_echo_color --switch=$(COLOR) --green --progress-dir=/home/manish/Music/Dolat-Hackathon/build/CMakeFiles --progress-num=$(CMAKE_PROGRESS_4) "Building CXX object CMakeFiles/app.out.dir/BACKEND/Module/Inventory/Inventory.cpp.o"
	/usr/bin/g++ $(CXX_DEFINES) $(CXX_INCLUDES) $(CXX_FLAGS) -MD -MT CMakeFiles/app.out.dir/BACKEND/Module/Inventory/Inventory.cpp.o -MF CMakeFiles/app.out.dir/BACKEND/Module/Inventory/Inventory.cpp.o.d -o CMakeFiles/app.out.dir/BACKEND/Module/Inventory/Inventory.cpp.o -c /home/manish/Music/Dolat-Hackathon/BACKEND/Module/Inventory/Inventory.cpp

CMakeFiles/app.out.dir/BACKEND/Module/Inventory/Inventory.cpp.i: cmake_force
	@$(CMAKE_COMMAND) -E cmake_echo_color --switch=$(COLOR) --green "Preprocessing CXX source to CMakeFiles/app.out.dir/BACKEND/Module/Inventory/Inventory.cpp.i"
	/usr/bin/g++ $(CXX_DEFINES) $(CXX_INCLUDES) $(CXX_FLAGS) -E /home/manish/Music/Dolat-Hackathon/BACKEND/Module/Inventory/Inventory.cpp > CMakeFiles/app.out.dir/BACKEND/Module/Inventory/Inventory.cpp.i

CMakeFiles/app.out.dir/BACKEND/Module/Inventory/Inventory.cpp.s: cmake_force
	@$(CMAKE_COMMAND) -E cmake_echo_color --switch=$(COLOR) --green "Compiling CXX source to assembly CMakeFiles/app.out.dir/BACKEND/Module/Inventory/Inventory.cpp.s"
	/usr/bin/g++ $(CXX_DEFINES) $(CXX_INCLUDES) $(CXX_FLAGS) -S /home/manish/Music/Dolat-Hackathon/BACKEND/Module/Inventory/Inventory.cpp -o CMakeFiles/app.out.dir/BACKEND/Module/Inventory/Inventory.cpp.s

CMakeFiles/app.out.dir/BACKEND/User/Admin/Admin.cpp.o: CMakeFiles/app.out.dir/flags.make
CMakeFiles/app.out.dir/BACKEND/User/Admin/Admin.cpp.o: ../BACKEND/User/Admin/Admin.cpp
CMakeFiles/app.out.dir/BACKEND/User/Admin/Admin.cpp.o: CMakeFiles/app.out.dir/compiler_depend.ts
	@$(CMAKE_COMMAND) -E cmake_echo_color --switch=$(COLOR) --green --progress-dir=/home/manish/Music/Dolat-Hackathon/build/CMakeFiles --progress-num=$(CMAKE_PROGRESS_5) "Building CXX object CMakeFiles/app.out.dir/BACKEND/User/Admin/Admin.cpp.o"
	/usr/bin/g++ $(CXX_DEFINES) $(CXX_INCLUDES) $(CXX_FLAGS) -MD -MT CMakeFiles/app.out.dir/BACKEND/User/Admin/Admin.cpp.o -MF CMakeFiles/app.out.dir/BACKEND/User/Admin/Admin.cpp.o.d -o CMakeFiles/app.out.dir/BACKEND/User/Admin/Admin.cpp.o -c /home/manish/Music/Dolat-Hackathon/BACKEND/User/Admin/Admin.cpp

CMakeFiles/app.out.dir/BACKEND/User/Admin/Admin.cpp.i: cmake_force
	@$(CMAKE_COMMAND) -E cmake_echo_color --switch=$(COLOR) --green "Preprocessing CXX source to CMakeFiles/app.out.dir/BACKEND/User/Admin/Admin.cpp.i"
	/usr/bin/g++ $(CXX_DEFINES) $(CXX_INCLUDES) $(CXX_FLAGS) -E /home/manish/Music/Dolat-Hackathon/BACKEND/User/Admin/Admin.cpp > CMakeFiles/app.out.dir/BACKEND/User/Admin/Admin.cpp.i

CMakeFiles/app.out.dir/BACKEND/User/Admin/Admin.cpp.s: cmake_force
	@$(CMAKE_COMMAND) -E cmake_echo_color --switch=$(COLOR) --green "Compiling CXX source to assembly CMakeFiles/app.out.dir/BACKEND/User/Admin/Admin.cpp.s"
	/usr/bin/g++ $(CXX_DEFINES) $(CXX_INCLUDES) $(CXX_FLAGS) -S /home/manish/Music/Dolat-Hackathon/BACKEND/User/Admin/Admin.cpp -o CMakeFiles/app.out.dir/BACKEND/User/Admin/Admin.cpp.s

CMakeFiles/app.out.dir/BACKEND/User/User.cpp.o: CMakeFiles/app.out.dir/flags.make
CMakeFiles/app.out.dir/BACKEND/User/User.cpp.o: ../BACKEND/User/User.cpp
CMakeFiles/app.out.dir/BACKEND/User/User.cpp.o: CMakeFiles/app.out.dir/compiler_depend.ts
	@$(CMAKE_COMMAND) -E cmake_echo_color --switch=$(COLOR) --green --progress-dir=/home/manish/Music/Dolat-Hackathon/build/CMakeFiles --progress-num=$(CMAKE_PROGRESS_6) "Building CXX object CMakeFiles/app.out.dir/BACKEND/User/User.cpp.o"
	/usr/bin/g++ $(CXX_DEFINES) $(CXX_INCLUDES) $(CXX_FLAGS) -MD -MT CMakeFiles/app.out.dir/BACKEND/User/User.cpp.o -MF CMakeFiles/app.out.dir/BACKEND/User/User.cpp.o.d -o CMakeFiles/app.out.dir/BACKEND/User/User.cpp.o -c /home/manish/Music/Dolat-Hackathon/BACKEND/User/User.cpp

CMakeFiles/app.out.dir/BACKEND/User/User.cpp.i: cmake_force
	@$(CMAKE_COMMAND) -E cmake_echo_color --switch=$(COLOR) --green "Preprocessing CXX source to CMakeFiles/app.out.dir/BACKEND/User/User.cpp.i"
	/usr/bin/g++ $(CXX_DEFINES) $(CXX_INCLUDES) $(CXX_FLAGS) -E /home/manish/Music/Dolat-Hackathon/BACKEND/User/User.cpp > CMakeFiles/app.out.dir/BACKEND/User/User.cpp.i

CMakeFiles/app.out.dir/BACKEND/User/User.cpp.s: cmake_force
	@$(CMAKE_COMMAND) -E cmake_echo_color --switch=$(COLOR) --green "Compiling CXX source to assembly CMakeFiles/app.out.dir/BACKEND/User/User.cpp.s"
	/usr/bin/g++ $(CXX_DEFINES) $(CXX_INCLUDES) $(CXX_FLAGS) -S /home/manish/Music/Dolat-Hackathon/BACKEND/User/User.cpp -o CMakeFiles/app.out.dir/BACKEND/User/User.cpp.s

CMakeFiles/app.out.dir/BACKEND/Utility/Project/Project.cpp.o: CMakeFiles/app.out.dir/flags.make
CMakeFiles/app.out.dir/BACKEND/Utility/Project/Project.cpp.o: ../BACKEND/Utility/Project/Project.cpp
CMakeFiles/app.out.dir/BACKEND/Utility/Project/Project.cpp.o: CMakeFiles/app.out.dir/compiler_depend.ts
	@$(CMAKE_COMMAND) -E cmake_echo_color --switch=$(COLOR) --green --progress-dir=/home/manish/Music/Dolat-Hackathon/build/CMakeFiles --progress-num=$(CMAKE_PROGRESS_7) "Building CXX object CMakeFiles/app.out.dir/BACKEND/Utility/Project/Project.cpp.o"
	/usr/bin/g++ $(CXX_DEFINES) $(CXX_INCLUDES) $(CXX_FLAGS) -MD -MT CMakeFiles/app.out.dir/BACKEND/Utility/Project/Project.cpp.o -MF CMakeFiles/app.out.dir/BACKEND/Utility/Project/Project.cpp.o.d -o CMakeFiles/app.out.dir/BACKEND/Utility/Project/Project.cpp.o -c /home/manish/Music/Dolat-Hackathon/BACKEND/Utility/Project/Project.cpp

CMakeFiles/app.out.dir/BACKEND/Utility/Project/Project.cpp.i: cmake_force
	@$(CMAKE_COMMAND) -E cmake_echo_color --switch=$(COLOR) --green "Preprocessing CXX source to CMakeFiles/app.out.dir/BACKEND/Utility/Project/Project.cpp.i"
	/usr/bin/g++ $(CXX_DEFINES) $(CXX_INCLUDES) $(CXX_FLAGS) -E /home/manish/Music/Dolat-Hackathon/BACKEND/Utility/Project/Project.cpp > CMakeFiles/app.out.dir/BACKEND/Utility/Project/Project.cpp.i

CMakeFiles/app.out.dir/BACKEND/Utility/Project/Project.cpp.s: cmake_force
	@$(CMAKE_COMMAND) -E cmake_echo_color --switch=$(COLOR) --green "Compiling CXX source to assembly CMakeFiles/app.out.dir/BACKEND/Utility/Project/Project.cpp.s"
	/usr/bin/g++ $(CXX_DEFINES) $(CXX_INCLUDES) $(CXX_FLAGS) -S /home/manish/Music/Dolat-Hackathon/BACKEND/Utility/Project/Project.cpp -o CMakeFiles/app.out.dir/BACKEND/Utility/Project/Project.cpp.s

# Object files for target app.out
app_out_OBJECTS = \
"CMakeFiles/app.out.dir/BACKEND/server.cpp.o" \
"CMakeFiles/app.out.dir/BACKEND/Module/Billing/Billing.cpp.o" \
"CMakeFiles/app.out.dir/BACKEND/Module/Request/RequestManagement.cpp.o" \
"CMakeFiles/app.out.dir/BACKEND/Module/Inventory/Inventory.cpp.o" \
"CMakeFiles/app.out.dir/BACKEND/User/Admin/Admin.cpp.o" \
"CMakeFiles/app.out.dir/BACKEND/User/User.cpp.o" \
"CMakeFiles/app.out.dir/BACKEND/Utility/Project/Project.cpp.o"

# External object files for target app.out
app_out_EXTERNAL_OBJECTS =

app.out: CMakeFiles/app.out.dir/BACKEND/server.cpp.o
app.out: CMakeFiles/app.out.dir/BACKEND/Module/Billing/Billing.cpp.o
app.out: CMakeFiles/app.out.dir/BACKEND/Module/Request/RequestManagement.cpp.o
app.out: CMakeFiles/app.out.dir/BACKEND/Module/Inventory/Inventory.cpp.o
app.out: CMakeFiles/app.out.dir/BACKEND/User/Admin/Admin.cpp.o
app.out: CMakeFiles/app.out.dir/BACKEND/User/User.cpp.o
app.out: CMakeFiles/app.out.dir/BACKEND/Utility/Project/Project.cpp.o
app.out: CMakeFiles/app.out.dir/build.make
app.out: CMakeFiles/app.out.dir/link.txt
	@$(CMAKE_COMMAND) -E cmake_echo_color --switch=$(COLOR) --green --bold --progress-dir=/home/manish/Music/Dolat-Hackathon/build/CMakeFiles --progress-num=$(CMAKE_PROGRESS_8) "Linking CXX executable app.out"
	$(CMAKE_COMMAND) -E cmake_link_script CMakeFiles/app.out.dir/link.txt --verbose=$(VERBOSE)

# Rule to build all files generated by this target.
CMakeFiles/app.out.dir/build: app.out
.PHONY : CMakeFiles/app.out.dir/build

CMakeFiles/app.out.dir/clean:
	$(CMAKE_COMMAND) -P CMakeFiles/app.out.dir/cmake_clean.cmake
.PHONY : CMakeFiles/app.out.dir/clean

CMakeFiles/app.out.dir/depend:
	cd /home/manish/Music/Dolat-Hackathon/build && $(CMAKE_COMMAND) -E cmake_depends "Unix Makefiles" /home/manish/Music/Dolat-Hackathon /home/manish/Music/Dolat-Hackathon /home/manish/Music/Dolat-Hackathon/build /home/manish/Music/Dolat-Hackathon/build /home/manish/Music/Dolat-Hackathon/build/CMakeFiles/app.out.dir/DependInfo.cmake --color=$(COLOR)
.PHONY : CMakeFiles/app.out.dir/depend

