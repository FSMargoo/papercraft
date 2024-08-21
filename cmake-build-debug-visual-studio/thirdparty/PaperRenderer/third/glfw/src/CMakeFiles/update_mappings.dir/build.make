# CMAKE generated file: DO NOT EDIT!
# Generated by "NMake Makefiles" Generator, CMake Version 3.29

# Delete rule output on recipe failure.
.DELETE_ON_ERROR:

#=============================================================================
# Special targets provided by cmake.

# Disable implicit rules so canonical targets will work.
.SUFFIXES:

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

!IF "$(OS)" == "Windows_NT"
NULL=
!ELSE
NULL=nul
!ENDIF
SHELL = cmd.exe

# The CMake executable.
CMAKE_COMMAND = "C:\Program Files\JetBrains\CLion 2024.2.0.1\bin\cmake\win\x64\bin\cmake.exe"

# The command to remove a file.
RM = "C:\Program Files\JetBrains\CLion 2024.2.0.1\bin\cmake\win\x64\bin\cmake.exe" -E rm -f

# Escaping for special characters.
EQUALS = =

# The top-level source directory on which CMake was run.
CMAKE_SOURCE_DIR = D:\papercraft

# The top-level build directory on which CMake was run.
CMAKE_BINARY_DIR = D:\papercraft\cmake-build-debug-visual-studio

# Utility rule file for update_mappings.

# Include any custom commands dependencies for this target.
include thirdparty\PaperRenderer\third\glfw\src\CMakeFiles\update_mappings.dir\compiler_depend.make

# Include the progress variables for this target.
include thirdparty\PaperRenderer\third\glfw\src\CMakeFiles\update_mappings.dir\progress.make

thirdparty\PaperRenderer\third\glfw\src\CMakeFiles\update_mappings:
	@$(CMAKE_COMMAND) -E cmake_echo_color "--switch=$(COLOR)" --blue --bold --progress-dir=D:\papercraft\cmake-build-debug-visual-studio\CMakeFiles --progress-num=$(CMAKE_PROGRESS_1) "Updating gamepad mappings from upstream repository"
	cd D:\papercraft\thirdparty\PaperRenderer\third\glfw\src
	echo >nul && "C:\Program Files\JetBrains\CLion 2024.2.0.1\bin\cmake\win\x64\bin\cmake.exe" -P D:/papercraft/thirdparty/PaperRenderer/third/glfw/CMake/GenerateMappings.cmake mappings.h.in mappings.h
	cd D:\papercraft\cmake-build-debug-visual-studio

update_mappings: thirdparty\PaperRenderer\third\glfw\src\CMakeFiles\update_mappings
update_mappings: thirdparty\PaperRenderer\third\glfw\src\CMakeFiles\update_mappings.dir\build.make
.PHONY : update_mappings

# Rule to build all files generated by this target.
thirdparty\PaperRenderer\third\glfw\src\CMakeFiles\update_mappings.dir\build: update_mappings
.PHONY : thirdparty\PaperRenderer\third\glfw\src\CMakeFiles\update_mappings.dir\build

thirdparty\PaperRenderer\third\glfw\src\CMakeFiles\update_mappings.dir\clean:
	cd D:\papercraft\cmake-build-debug-visual-studio\thirdparty\PaperRenderer\third\glfw\src
	$(CMAKE_COMMAND) -P CMakeFiles\update_mappings.dir\cmake_clean.cmake
	cd D:\papercraft\cmake-build-debug-visual-studio
.PHONY : thirdparty\PaperRenderer\third\glfw\src\CMakeFiles\update_mappings.dir\clean

thirdparty\PaperRenderer\third\glfw\src\CMakeFiles\update_mappings.dir\depend:
	$(CMAKE_COMMAND) -E cmake_depends "NMake Makefiles" D:\papercraft D:\papercraft\thirdparty\PaperRenderer\third\glfw\src D:\papercraft\cmake-build-debug-visual-studio D:\papercraft\cmake-build-debug-visual-studio\thirdparty\PaperRenderer\third\glfw\src D:\papercraft\cmake-build-debug-visual-studio\thirdparty\PaperRenderer\third\glfw\src\CMakeFiles\update_mappings.dir\DependInfo.cmake "--color=$(COLOR)"
.PHONY : thirdparty\PaperRenderer\third\glfw\src\CMakeFiles\update_mappings.dir\depend

