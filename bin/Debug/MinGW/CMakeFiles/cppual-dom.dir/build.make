# CMAKE generated file: DO NOT EDIT!
# Generated by "MinGW Makefiles" Generator, CMake Version 3.2

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

SHELL = cmd.exe

# The CMake executable.
CMAKE_COMMAND = "C:\Program Files (x86)\CMake\bin\cmake.exe"

# The command to remove a file.
RM = "C:\Program Files (x86)\CMake\bin\cmake.exe" -E remove -f

# Escaping for special characters.
EQUALS = =

# The top-level source directory on which CMake was run.
CMAKE_SOURCE_DIR = E:\Development\c++\projects\cross\cppual

# The top-level build directory on which CMake was run.
CMAKE_BINARY_DIR = E:\Development\c++\projects\cross\cppual\bin\Debug\MinGW

# Include any dependencies generated for this target.
include CMakeFiles/cppual-dom.dir/depend.make

# Include the progress variables for this target.
include CMakeFiles/cppual-dom.dir/progress.make

# Include the compile flags for this target's objects.
include CMakeFiles/cppual-dom.dir/flags.make

CMakeFiles/cppual-dom.dir/src/dom/xml.cpp.obj: CMakeFiles/cppual-dom.dir/flags.make
CMakeFiles/cppual-dom.dir/src/dom/xml.cpp.obj: CMakeFiles/cppual-dom.dir/includes_CXX.rsp
CMakeFiles/cppual-dom.dir/src/dom/xml.cpp.obj: ../../../src/dom/xml.cpp
	$(CMAKE_COMMAND) -E cmake_progress_report E:\Development\c++\projects\cross\cppual\bin\Debug\MinGW\CMakeFiles $(CMAKE_PROGRESS_1)
	@$(CMAKE_COMMAND) -E cmake_echo_color --switch=$(COLOR) --green "Building CXX object CMakeFiles/cppual-dom.dir/src/dom/xml.cpp.obj"
	C:\Qt\Tools\mingw491_32\bin\g++.exe   $(CXX_DEFINES) $(CXX_FLAGS) -o CMakeFiles\cppual-dom.dir\src\dom\xml.cpp.obj -c E:\Development\c++\projects\cross\cppual\src\dom\xml.cpp

CMakeFiles/cppual-dom.dir/src/dom/xml.cpp.i: cmake_force
	@$(CMAKE_COMMAND) -E cmake_echo_color --switch=$(COLOR) --green "Preprocessing CXX source to CMakeFiles/cppual-dom.dir/src/dom/xml.cpp.i"
	C:\Qt\Tools\mingw491_32\bin\g++.exe  $(CXX_DEFINES) $(CXX_FLAGS) -E E:\Development\c++\projects\cross\cppual\src\dom\xml.cpp > CMakeFiles\cppual-dom.dir\src\dom\xml.cpp.i

CMakeFiles/cppual-dom.dir/src/dom/xml.cpp.s: cmake_force
	@$(CMAKE_COMMAND) -E cmake_echo_color --switch=$(COLOR) --green "Compiling CXX source to assembly CMakeFiles/cppual-dom.dir/src/dom/xml.cpp.s"
	C:\Qt\Tools\mingw491_32\bin\g++.exe  $(CXX_DEFINES) $(CXX_FLAGS) -S E:\Development\c++\projects\cross\cppual\src\dom\xml.cpp -o CMakeFiles\cppual-dom.dir\src\dom\xml.cpp.s

CMakeFiles/cppual-dom.dir/src/dom/xml.cpp.obj.requires:
.PHONY : CMakeFiles/cppual-dom.dir/src/dom/xml.cpp.obj.requires

CMakeFiles/cppual-dom.dir/src/dom/xml.cpp.obj.provides: CMakeFiles/cppual-dom.dir/src/dom/xml.cpp.obj.requires
	$(MAKE) -f CMakeFiles\cppual-dom.dir\build.make CMakeFiles/cppual-dom.dir/src/dom/xml.cpp.obj.provides.build
.PHONY : CMakeFiles/cppual-dom.dir/src/dom/xml.cpp.obj.provides

CMakeFiles/cppual-dom.dir/src/dom/xml.cpp.obj.provides.build: CMakeFiles/cppual-dom.dir/src/dom/xml.cpp.obj

# Object files for target cppual-dom
cppual__dom_OBJECTS = \
"CMakeFiles/cppual-dom.dir/src/dom/xml.cpp.obj"

# External object files for target cppual-dom
cppual__dom_EXTERNAL_OBJECTS =

libcppual-dom.dll: CMakeFiles/cppual-dom.dir/src/dom/xml.cpp.obj
libcppual-dom.dll: CMakeFiles/cppual-dom.dir/build.make
libcppual-dom.dll: CMakeFiles/cppual-dom.dir/linklibs.rsp
libcppual-dom.dll: CMakeFiles/cppual-dom.dir/objects1.rsp
libcppual-dom.dll: CMakeFiles/cppual-dom.dir/link.txt
	@$(CMAKE_COMMAND) -E cmake_echo_color --switch=$(COLOR) --red --bold "Linking CXX shared library libcppual-dom.dll"
	$(CMAKE_COMMAND) -E cmake_link_script CMakeFiles\cppual-dom.dir\link.txt --verbose=$(VERBOSE)

# Rule to build all files generated by this target.
CMakeFiles/cppual-dom.dir/build: libcppual-dom.dll
.PHONY : CMakeFiles/cppual-dom.dir/build

CMakeFiles/cppual-dom.dir/requires: CMakeFiles/cppual-dom.dir/src/dom/xml.cpp.obj.requires
.PHONY : CMakeFiles/cppual-dom.dir/requires

CMakeFiles/cppual-dom.dir/clean:
	$(CMAKE_COMMAND) -P CMakeFiles\cppual-dom.dir\cmake_clean.cmake
.PHONY : CMakeFiles/cppual-dom.dir/clean

CMakeFiles/cppual-dom.dir/depend:
	$(CMAKE_COMMAND) -E cmake_depends "MinGW Makefiles" E:\Development\c++\projects\cross\cppual E:\Development\c++\projects\cross\cppual E:\Development\c++\projects\cross\cppual\bin\Debug\MinGW E:\Development\c++\projects\cross\cppual\bin\Debug\MinGW E:\Development\c++\projects\cross\cppual\bin\Debug\MinGW\CMakeFiles\cppual-dom.dir\DependInfo.cmake --color=$(COLOR)
.PHONY : CMakeFiles/cppual-dom.dir/depend

