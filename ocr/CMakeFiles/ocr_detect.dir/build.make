# CMAKE generated file: DO NOT EDIT!
# Generated by "Unix Makefiles" Generator, CMake Version 2.8

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

# The program to use to edit the cache.
CMAKE_EDIT_COMMAND = /usr/bin/cmake-gui

# The top-level source directory on which CMake was run.
CMAKE_SOURCE_DIR = /home/cloud/cloud-git-master/opencv/ocr

# The top-level build directory on which CMake was run.
CMAKE_BINARY_DIR = /home/cloud/cloud-git-master/opencv/ocr

# Include any dependencies generated for this target.
include CMakeFiles/ocr_detect.dir/depend.make

# Include the progress variables for this target.
include CMakeFiles/ocr_detect.dir/progress.make

# Include the compile flags for this target's objects.
include CMakeFiles/ocr_detect.dir/flags.make

CMakeFiles/ocr_detect.dir/src/ocr_java.cpp.o: CMakeFiles/ocr_detect.dir/flags.make
CMakeFiles/ocr_detect.dir/src/ocr_java.cpp.o: src/ocr_java.cpp
	$(CMAKE_COMMAND) -E cmake_progress_report /home/cloud/cloud-git-master/opencv/ocr/CMakeFiles $(CMAKE_PROGRESS_1)
	@$(CMAKE_COMMAND) -E cmake_echo_color --switch=$(COLOR) --green "Building CXX object CMakeFiles/ocr_detect.dir/src/ocr_java.cpp.o"
	/usr/bin/c++   $(CXX_DEFINES) $(CXX_FLAGS) -o CMakeFiles/ocr_detect.dir/src/ocr_java.cpp.o -c /home/cloud/cloud-git-master/opencv/ocr/src/ocr_java.cpp

CMakeFiles/ocr_detect.dir/src/ocr_java.cpp.i: cmake_force
	@$(CMAKE_COMMAND) -E cmake_echo_color --switch=$(COLOR) --green "Preprocessing CXX source to CMakeFiles/ocr_detect.dir/src/ocr_java.cpp.i"
	/usr/bin/c++  $(CXX_DEFINES) $(CXX_FLAGS) -E /home/cloud/cloud-git-master/opencv/ocr/src/ocr_java.cpp > CMakeFiles/ocr_detect.dir/src/ocr_java.cpp.i

CMakeFiles/ocr_detect.dir/src/ocr_java.cpp.s: cmake_force
	@$(CMAKE_COMMAND) -E cmake_echo_color --switch=$(COLOR) --green "Compiling CXX source to assembly CMakeFiles/ocr_detect.dir/src/ocr_java.cpp.s"
	/usr/bin/c++  $(CXX_DEFINES) $(CXX_FLAGS) -S /home/cloud/cloud-git-master/opencv/ocr/src/ocr_java.cpp -o CMakeFiles/ocr_detect.dir/src/ocr_java.cpp.s

CMakeFiles/ocr_detect.dir/src/ocr_java.cpp.o.requires:
.PHONY : CMakeFiles/ocr_detect.dir/src/ocr_java.cpp.o.requires

CMakeFiles/ocr_detect.dir/src/ocr_java.cpp.o.provides: CMakeFiles/ocr_detect.dir/src/ocr_java.cpp.o.requires
	$(MAKE) -f CMakeFiles/ocr_detect.dir/build.make CMakeFiles/ocr_detect.dir/src/ocr_java.cpp.o.provides.build
.PHONY : CMakeFiles/ocr_detect.dir/src/ocr_java.cpp.o.provides

CMakeFiles/ocr_detect.dir/src/ocr_java.cpp.o.provides.build: CMakeFiles/ocr_detect.dir/src/ocr_java.cpp.o

CMakeFiles/ocr_detect.dir/src/ocr_read.cpp.o: CMakeFiles/ocr_detect.dir/flags.make
CMakeFiles/ocr_detect.dir/src/ocr_read.cpp.o: src/ocr_read.cpp
	$(CMAKE_COMMAND) -E cmake_progress_report /home/cloud/cloud-git-master/opencv/ocr/CMakeFiles $(CMAKE_PROGRESS_2)
	@$(CMAKE_COMMAND) -E cmake_echo_color --switch=$(COLOR) --green "Building CXX object CMakeFiles/ocr_detect.dir/src/ocr_read.cpp.o"
	/usr/bin/c++   $(CXX_DEFINES) $(CXX_FLAGS) -o CMakeFiles/ocr_detect.dir/src/ocr_read.cpp.o -c /home/cloud/cloud-git-master/opencv/ocr/src/ocr_read.cpp

CMakeFiles/ocr_detect.dir/src/ocr_read.cpp.i: cmake_force
	@$(CMAKE_COMMAND) -E cmake_echo_color --switch=$(COLOR) --green "Preprocessing CXX source to CMakeFiles/ocr_detect.dir/src/ocr_read.cpp.i"
	/usr/bin/c++  $(CXX_DEFINES) $(CXX_FLAGS) -E /home/cloud/cloud-git-master/opencv/ocr/src/ocr_read.cpp > CMakeFiles/ocr_detect.dir/src/ocr_read.cpp.i

CMakeFiles/ocr_detect.dir/src/ocr_read.cpp.s: cmake_force
	@$(CMAKE_COMMAND) -E cmake_echo_color --switch=$(COLOR) --green "Compiling CXX source to assembly CMakeFiles/ocr_detect.dir/src/ocr_read.cpp.s"
	/usr/bin/c++  $(CXX_DEFINES) $(CXX_FLAGS) -S /home/cloud/cloud-git-master/opencv/ocr/src/ocr_read.cpp -o CMakeFiles/ocr_detect.dir/src/ocr_read.cpp.s

CMakeFiles/ocr_detect.dir/src/ocr_read.cpp.o.requires:
.PHONY : CMakeFiles/ocr_detect.dir/src/ocr_read.cpp.o.requires

CMakeFiles/ocr_detect.dir/src/ocr_read.cpp.o.provides: CMakeFiles/ocr_detect.dir/src/ocr_read.cpp.o.requires
	$(MAKE) -f CMakeFiles/ocr_detect.dir/build.make CMakeFiles/ocr_detect.dir/src/ocr_read.cpp.o.provides.build
.PHONY : CMakeFiles/ocr_detect.dir/src/ocr_read.cpp.o.provides

CMakeFiles/ocr_detect.dir/src/ocr_read.cpp.o.provides.build: CMakeFiles/ocr_detect.dir/src/ocr_read.cpp.o

CMakeFiles/ocr_detect.dir/src/ocr_tesseract.cpp.o: CMakeFiles/ocr_detect.dir/flags.make
CMakeFiles/ocr_detect.dir/src/ocr_tesseract.cpp.o: src/ocr_tesseract.cpp
	$(CMAKE_COMMAND) -E cmake_progress_report /home/cloud/cloud-git-master/opencv/ocr/CMakeFiles $(CMAKE_PROGRESS_3)
	@$(CMAKE_COMMAND) -E cmake_echo_color --switch=$(COLOR) --green "Building CXX object CMakeFiles/ocr_detect.dir/src/ocr_tesseract.cpp.o"
	/usr/bin/c++   $(CXX_DEFINES) $(CXX_FLAGS) -o CMakeFiles/ocr_detect.dir/src/ocr_tesseract.cpp.o -c /home/cloud/cloud-git-master/opencv/ocr/src/ocr_tesseract.cpp

CMakeFiles/ocr_detect.dir/src/ocr_tesseract.cpp.i: cmake_force
	@$(CMAKE_COMMAND) -E cmake_echo_color --switch=$(COLOR) --green "Preprocessing CXX source to CMakeFiles/ocr_detect.dir/src/ocr_tesseract.cpp.i"
	/usr/bin/c++  $(CXX_DEFINES) $(CXX_FLAGS) -E /home/cloud/cloud-git-master/opencv/ocr/src/ocr_tesseract.cpp > CMakeFiles/ocr_detect.dir/src/ocr_tesseract.cpp.i

CMakeFiles/ocr_detect.dir/src/ocr_tesseract.cpp.s: cmake_force
	@$(CMAKE_COMMAND) -E cmake_echo_color --switch=$(COLOR) --green "Compiling CXX source to assembly CMakeFiles/ocr_detect.dir/src/ocr_tesseract.cpp.s"
	/usr/bin/c++  $(CXX_DEFINES) $(CXX_FLAGS) -S /home/cloud/cloud-git-master/opencv/ocr/src/ocr_tesseract.cpp -o CMakeFiles/ocr_detect.dir/src/ocr_tesseract.cpp.s

CMakeFiles/ocr_detect.dir/src/ocr_tesseract.cpp.o.requires:
.PHONY : CMakeFiles/ocr_detect.dir/src/ocr_tesseract.cpp.o.requires

CMakeFiles/ocr_detect.dir/src/ocr_tesseract.cpp.o.provides: CMakeFiles/ocr_detect.dir/src/ocr_tesseract.cpp.o.requires
	$(MAKE) -f CMakeFiles/ocr_detect.dir/build.make CMakeFiles/ocr_detect.dir/src/ocr_tesseract.cpp.o.provides.build
.PHONY : CMakeFiles/ocr_detect.dir/src/ocr_tesseract.cpp.o.provides

CMakeFiles/ocr_detect.dir/src/ocr_tesseract.cpp.o.provides.build: CMakeFiles/ocr_detect.dir/src/ocr_tesseract.cpp.o

CMakeFiles/ocr_detect.dir/src/ocr_main.cpp.o: CMakeFiles/ocr_detect.dir/flags.make
CMakeFiles/ocr_detect.dir/src/ocr_main.cpp.o: src/ocr_main.cpp
	$(CMAKE_COMMAND) -E cmake_progress_report /home/cloud/cloud-git-master/opencv/ocr/CMakeFiles $(CMAKE_PROGRESS_4)
	@$(CMAKE_COMMAND) -E cmake_echo_color --switch=$(COLOR) --green "Building CXX object CMakeFiles/ocr_detect.dir/src/ocr_main.cpp.o"
	/usr/bin/c++   $(CXX_DEFINES) $(CXX_FLAGS) -o CMakeFiles/ocr_detect.dir/src/ocr_main.cpp.o -c /home/cloud/cloud-git-master/opencv/ocr/src/ocr_main.cpp

CMakeFiles/ocr_detect.dir/src/ocr_main.cpp.i: cmake_force
	@$(CMAKE_COMMAND) -E cmake_echo_color --switch=$(COLOR) --green "Preprocessing CXX source to CMakeFiles/ocr_detect.dir/src/ocr_main.cpp.i"
	/usr/bin/c++  $(CXX_DEFINES) $(CXX_FLAGS) -E /home/cloud/cloud-git-master/opencv/ocr/src/ocr_main.cpp > CMakeFiles/ocr_detect.dir/src/ocr_main.cpp.i

CMakeFiles/ocr_detect.dir/src/ocr_main.cpp.s: cmake_force
	@$(CMAKE_COMMAND) -E cmake_echo_color --switch=$(COLOR) --green "Compiling CXX source to assembly CMakeFiles/ocr_detect.dir/src/ocr_main.cpp.s"
	/usr/bin/c++  $(CXX_DEFINES) $(CXX_FLAGS) -S /home/cloud/cloud-git-master/opencv/ocr/src/ocr_main.cpp -o CMakeFiles/ocr_detect.dir/src/ocr_main.cpp.s

CMakeFiles/ocr_detect.dir/src/ocr_main.cpp.o.requires:
.PHONY : CMakeFiles/ocr_detect.dir/src/ocr_main.cpp.o.requires

CMakeFiles/ocr_detect.dir/src/ocr_main.cpp.o.provides: CMakeFiles/ocr_detect.dir/src/ocr_main.cpp.o.requires
	$(MAKE) -f CMakeFiles/ocr_detect.dir/build.make CMakeFiles/ocr_detect.dir/src/ocr_main.cpp.o.provides.build
.PHONY : CMakeFiles/ocr_detect.dir/src/ocr_main.cpp.o.provides

CMakeFiles/ocr_detect.dir/src/ocr_main.cpp.o.provides.build: CMakeFiles/ocr_detect.dir/src/ocr_main.cpp.o

# Object files for target ocr_detect
ocr_detect_OBJECTS = \
"CMakeFiles/ocr_detect.dir/src/ocr_java.cpp.o" \
"CMakeFiles/ocr_detect.dir/src/ocr_read.cpp.o" \
"CMakeFiles/ocr_detect.dir/src/ocr_tesseract.cpp.o" \
"CMakeFiles/ocr_detect.dir/src/ocr_main.cpp.o"

# External object files for target ocr_detect
ocr_detect_EXTERNAL_OBJECTS =

ocr_detect: CMakeFiles/ocr_detect.dir/src/ocr_java.cpp.o
ocr_detect: CMakeFiles/ocr_detect.dir/src/ocr_read.cpp.o
ocr_detect: CMakeFiles/ocr_detect.dir/src/ocr_tesseract.cpp.o
ocr_detect: CMakeFiles/ocr_detect.dir/src/ocr_main.cpp.o
ocr_detect: CMakeFiles/ocr_detect.dir/build.make
ocr_detect: /home/cloud/opencv3.0/opencv-3.0.0-alpha/release/lib/libopencv_videostab.so.3.0.0
ocr_detect: /home/cloud/opencv3.0/opencv-3.0.0-alpha/release/lib/libopencv_videoio.so.3.0.0
ocr_detect: /home/cloud/opencv3.0/opencv-3.0.0-alpha/release/lib/libopencv_video.so.3.0.0
ocr_detect: /home/cloud/opencv3.0/opencv-3.0.0-alpha/release/lib/libopencv_ts.a
ocr_detect: /home/cloud/opencv3.0/opencv-3.0.0-alpha/release/lib/libopencv_superres.so.3.0.0
ocr_detect: /home/cloud/opencv3.0/opencv-3.0.0-alpha/release/lib/libopencv_stitching.so.3.0.0
ocr_detect: /home/cloud/opencv3.0/opencv-3.0.0-alpha/release/lib/libopencv_shape.so.3.0.0
ocr_detect: /home/cloud/opencv3.0/opencv-3.0.0-alpha/release/lib/libopencv_photo.so.3.0.0
ocr_detect: /home/cloud/opencv3.0/opencv-3.0.0-alpha/release/lib/libopencv_objdetect.so.3.0.0
ocr_detect: /home/cloud/opencv3.0/opencv-3.0.0-alpha/release/lib/libopencv_ml.so.3.0.0
ocr_detect: /home/cloud/opencv3.0/opencv-3.0.0-alpha/release/lib/libopencv_imgproc.so.3.0.0
ocr_detect: /home/cloud/opencv3.0/opencv-3.0.0-alpha/release/lib/libopencv_imgcodecs.so.3.0.0
ocr_detect: /home/cloud/opencv3.0/opencv-3.0.0-alpha/release/lib/libopencv_highgui.so.3.0.0
ocr_detect: /home/cloud/opencv3.0/opencv-3.0.0-alpha/release/lib/libopencv_flann.so.3.0.0
ocr_detect: /home/cloud/opencv3.0/opencv-3.0.0-alpha/release/lib/libopencv_features2d.so.3.0.0
ocr_detect: /home/cloud/opencv3.0/opencv-3.0.0-alpha/release/lib/libopencv_core.so.3.0.0
ocr_detect: /home/cloud/opencv3.0/opencv-3.0.0-alpha/release/lib/libopencv_calib3d.so.3.0.0
ocr_detect: /usr/lib/x86_64-linux-gnu/libMagickCore.so
ocr_detect: /home/cloud/opencv3.0/opencv-3.0.0-alpha/release/../3rdparty/ippicv/unpack/ippicv_lnx/lib/intel64/libippicv.a
ocr_detect: /home/cloud/opencv3.0/opencv-3.0.0-alpha/release/lib/libopencv_features2d.so.3.0.0
ocr_detect: /home/cloud/opencv3.0/opencv-3.0.0-alpha/release/lib/libopencv_highgui.so.3.0.0
ocr_detect: /home/cloud/opencv3.0/opencv-3.0.0-alpha/release/lib/libopencv_videoio.so.3.0.0
ocr_detect: /home/cloud/opencv3.0/opencv-3.0.0-alpha/release/lib/libopencv_imgcodecs.so.3.0.0
ocr_detect: /home/cloud/opencv3.0/opencv-3.0.0-alpha/release/lib/libopencv_flann.so.3.0.0
ocr_detect: /home/cloud/opencv3.0/opencv-3.0.0-alpha/release/lib/libopencv_video.so.3.0.0
ocr_detect: /home/cloud/opencv3.0/opencv-3.0.0-alpha/release/lib/libopencv_imgproc.so.3.0.0
ocr_detect: /home/cloud/opencv3.0/opencv-3.0.0-alpha/release/lib/libopencv_core.so.3.0.0
ocr_detect: CMakeFiles/ocr_detect.dir/link.txt
	@$(CMAKE_COMMAND) -E cmake_echo_color --switch=$(COLOR) --red --bold "Linking CXX executable ocr_detect"
	$(CMAKE_COMMAND) -E cmake_link_script CMakeFiles/ocr_detect.dir/link.txt --verbose=$(VERBOSE)

# Rule to build all files generated by this target.
CMakeFiles/ocr_detect.dir/build: ocr_detect
.PHONY : CMakeFiles/ocr_detect.dir/build

CMakeFiles/ocr_detect.dir/requires: CMakeFiles/ocr_detect.dir/src/ocr_java.cpp.o.requires
CMakeFiles/ocr_detect.dir/requires: CMakeFiles/ocr_detect.dir/src/ocr_read.cpp.o.requires
CMakeFiles/ocr_detect.dir/requires: CMakeFiles/ocr_detect.dir/src/ocr_tesseract.cpp.o.requires
CMakeFiles/ocr_detect.dir/requires: CMakeFiles/ocr_detect.dir/src/ocr_main.cpp.o.requires
.PHONY : CMakeFiles/ocr_detect.dir/requires

CMakeFiles/ocr_detect.dir/clean:
	$(CMAKE_COMMAND) -P CMakeFiles/ocr_detect.dir/cmake_clean.cmake
.PHONY : CMakeFiles/ocr_detect.dir/clean

CMakeFiles/ocr_detect.dir/depend:
	cd /home/cloud/cloud-git-master/opencv/ocr && $(CMAKE_COMMAND) -E cmake_depends "Unix Makefiles" /home/cloud/cloud-git-master/opencv/ocr /home/cloud/cloud-git-master/opencv/ocr /home/cloud/cloud-git-master/opencv/ocr /home/cloud/cloud-git-master/opencv/ocr /home/cloud/cloud-git-master/opencv/ocr/CMakeFiles/ocr_detect.dir/DependInfo.cmake --color=$(COLOR)
.PHONY : CMakeFiles/ocr_detect.dir/depend

