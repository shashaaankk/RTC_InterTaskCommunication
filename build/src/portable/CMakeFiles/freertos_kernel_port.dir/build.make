# CMAKE generated file: DO NOT EDIT!
# Generated by "MinGW Makefiles" Generator, CMake Version 3.28

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
CMAKE_COMMAND = "C:\Program Files\CMake\bin\cmake.exe"

# The command to remove a file.
RM = "C:\Program Files\CMake\bin\cmake.exe" -E rm -f

# Escaping for special characters.
EQUALS = =

# The top-level source directory on which CMake was run.
CMAKE_SOURCE_DIR = "S:\M.ScInformationTechnology-ES\Semester 1\05_RealTimeConceptsforEmbeddedSystems\intertask_communication"

# The top-level build directory on which CMake was run.
CMAKE_BINARY_DIR = "S:\M.ScInformationTechnology-ES\Semester 1\05_RealTimeConceptsforEmbeddedSystems\intertask_communication\build"

# Include any dependencies generated for this target.
include src/portable/CMakeFiles/freertos_kernel_port.dir/depend.make
# Include any dependencies generated by the compiler for this target.
include src/portable/CMakeFiles/freertos_kernel_port.dir/compiler_depend.make

# Include the progress variables for this target.
include src/portable/CMakeFiles/freertos_kernel_port.dir/progress.make

# Include the compile flags for this target's objects.
include src/portable/CMakeFiles/freertos_kernel_port.dir/flags.make

src/portable/CMakeFiles/freertos_kernel_port.dir/MSVC-MingW/port.c.obj: src/portable/CMakeFiles/freertos_kernel_port.dir/flags.make
src/portable/CMakeFiles/freertos_kernel_port.dir/MSVC-MingW/port.c.obj: src/portable/CMakeFiles/freertos_kernel_port.dir/includes_C.rsp
src/portable/CMakeFiles/freertos_kernel_port.dir/MSVC-MingW/port.c.obj: S:/M.ScInformationTechnology-ES/Semester\ 1/05_RealTimeConceptsforEmbeddedSystems/intertask_communication/src/portable/MSVC-MingW/port.c
src/portable/CMakeFiles/freertos_kernel_port.dir/MSVC-MingW/port.c.obj: src/portable/CMakeFiles/freertos_kernel_port.dir/compiler_depend.ts
	@$(CMAKE_COMMAND) -E cmake_echo_color "--switch=$(COLOR)" --green --progress-dir="S:\M.ScInformationTechnology-ES\Semester 1\05_RealTimeConceptsforEmbeddedSystems\intertask_communication\build\CMakeFiles" --progress-num=$(CMAKE_PROGRESS_1) "Building C object src/portable/CMakeFiles/freertos_kernel_port.dir/MSVC-MingW/port.c.obj"
	cd /d "S:\M.ScInformationTechnology-ES\Semester 1\05_RealTimeConceptsforEmbeddedSystems\intertask_communication\build\src\portable" && C:\msys64\mingw64\bin\cc.exe $(C_DEFINES) $(C_INCLUDES) $(C_FLAGS) -MD -MT src/portable/CMakeFiles/freertos_kernel_port.dir/MSVC-MingW/port.c.obj -MF CMakeFiles\freertos_kernel_port.dir\MSVC-MingW\port.c.obj.d -o CMakeFiles\freertos_kernel_port.dir\MSVC-MingW\port.c.obj -c "S:\M.ScInformationTechnology-ES\Semester 1\05_RealTimeConceptsforEmbeddedSystems\intertask_communication\src\portable\MSVC-MingW\port.c"

src/portable/CMakeFiles/freertos_kernel_port.dir/MSVC-MingW/port.c.i: cmake_force
	@$(CMAKE_COMMAND) -E cmake_echo_color "--switch=$(COLOR)" --green "Preprocessing C source to CMakeFiles/freertos_kernel_port.dir/MSVC-MingW/port.c.i"
	cd /d "S:\M.ScInformationTechnology-ES\Semester 1\05_RealTimeConceptsforEmbeddedSystems\intertask_communication\build\src\portable" && C:\msys64\mingw64\bin\cc.exe $(C_DEFINES) $(C_INCLUDES) $(C_FLAGS) -E "S:\M.ScInformationTechnology-ES\Semester 1\05_RealTimeConceptsforEmbeddedSystems\intertask_communication\src\portable\MSVC-MingW\port.c" > CMakeFiles\freertos_kernel_port.dir\MSVC-MingW\port.c.i

src/portable/CMakeFiles/freertos_kernel_port.dir/MSVC-MingW/port.c.s: cmake_force
	@$(CMAKE_COMMAND) -E cmake_echo_color "--switch=$(COLOR)" --green "Compiling C source to assembly CMakeFiles/freertos_kernel_port.dir/MSVC-MingW/port.c.s"
	cd /d "S:\M.ScInformationTechnology-ES\Semester 1\05_RealTimeConceptsforEmbeddedSystems\intertask_communication\build\src\portable" && C:\msys64\mingw64\bin\cc.exe $(C_DEFINES) $(C_INCLUDES) $(C_FLAGS) -S "S:\M.ScInformationTechnology-ES\Semester 1\05_RealTimeConceptsforEmbeddedSystems\intertask_communication\src\portable\MSVC-MingW\port.c" -o CMakeFiles\freertos_kernel_port.dir\MSVC-MingW\port.c.s

# Object files for target freertos_kernel_port
freertos_kernel_port_OBJECTS = \
"CMakeFiles/freertos_kernel_port.dir/MSVC-MingW/port.c.obj"

# External object files for target freertos_kernel_port
freertos_kernel_port_EXTERNAL_OBJECTS =

src/portable/libfreertos_kernel_port.a: src/portable/CMakeFiles/freertos_kernel_port.dir/MSVC-MingW/port.c.obj
src/portable/libfreertos_kernel_port.a: src/portable/CMakeFiles/freertos_kernel_port.dir/build.make
src/portable/libfreertos_kernel_port.a: src/portable/CMakeFiles/freertos_kernel_port.dir/link.txt
	@$(CMAKE_COMMAND) -E cmake_echo_color "--switch=$(COLOR)" --green --bold --progress-dir="S:\M.ScInformationTechnology-ES\Semester 1\05_RealTimeConceptsforEmbeddedSystems\intertask_communication\build\CMakeFiles" --progress-num=$(CMAKE_PROGRESS_2) "Linking C static library libfreertos_kernel_port.a"
	cd /d "S:\M.ScInformationTechnology-ES\Semester 1\05_RealTimeConceptsforEmbeddedSystems\intertask_communication\build\src\portable" && $(CMAKE_COMMAND) -P CMakeFiles\freertos_kernel_port.dir\cmake_clean_target.cmake
	cd /d "S:\M.ScInformationTechnology-ES\Semester 1\05_RealTimeConceptsforEmbeddedSystems\intertask_communication\build\src\portable" && $(CMAKE_COMMAND) -E cmake_link_script CMakeFiles\freertos_kernel_port.dir\link.txt --verbose=$(VERBOSE)

# Rule to build all files generated by this target.
src/portable/CMakeFiles/freertos_kernel_port.dir/build: src/portable/libfreertos_kernel_port.a
.PHONY : src/portable/CMakeFiles/freertos_kernel_port.dir/build

src/portable/CMakeFiles/freertos_kernel_port.dir/clean:
	cd /d "S:\M.ScInformationTechnology-ES\Semester 1\05_RealTimeConceptsforEmbeddedSystems\intertask_communication\build\src\portable" && $(CMAKE_COMMAND) -P CMakeFiles\freertos_kernel_port.dir\cmake_clean.cmake
.PHONY : src/portable/CMakeFiles/freertos_kernel_port.dir/clean

src/portable/CMakeFiles/freertos_kernel_port.dir/depend:
	$(CMAKE_COMMAND) -E cmake_depends "MinGW Makefiles" "S:\M.ScInformationTechnology-ES\Semester 1\05_RealTimeConceptsforEmbeddedSystems\intertask_communication" "S:\M.ScInformationTechnology-ES\Semester 1\05_RealTimeConceptsforEmbeddedSystems\intertask_communication\src\portable" "S:\M.ScInformationTechnology-ES\Semester 1\05_RealTimeConceptsforEmbeddedSystems\intertask_communication\build" "S:\M.ScInformationTechnology-ES\Semester 1\05_RealTimeConceptsforEmbeddedSystems\intertask_communication\build\src\portable" "S:\M.ScInformationTechnology-ES\Semester 1\05_RealTimeConceptsforEmbeddedSystems\intertask_communication\build\src\portable\CMakeFiles\freertos_kernel_port.dir\DependInfo.cmake" "--color=$(COLOR)"
.PHONY : src/portable/CMakeFiles/freertos_kernel_port.dir/depend

