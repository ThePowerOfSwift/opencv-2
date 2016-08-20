cmake_minimum_required(VERSION 2.8.12.2)
project(ocr)
set(OpenCV_DIR /home/cloud/opencv3.0/opencv-3.0.0-alpha/release)
include_directories(/home/cloud/opencv_lib/include)
include_directories(./)
include_directories(/usr/local/include/leptonica)
include_directories(/usr/lib/jvm/java-7-openjdk-amd64/include/)
find_package(OpenCV REQUIRED)
link_directories(/usr/local/lib)
set(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} -std=c++11")

include(LibFindMacros)
# Dependencies
#libfind_package(MagickCore Magick)
# Use pkg-config to get hints about paths
libfind_pkg_check_modules(MagickCore_PKGCONF MagickCore)

# Include dir
find_path(MagickCore_INCLUDE_DIR
        NAMES MagickCore.h
        PATHS ${MagickCore_PKGCONF_INCLUDE_DIRS}
        )
message(STATUS "    include path: ${MagickCore_PKGCONF_INCLUDE_DIRS}")
message(STATUS "    include path: ${MagickCore_INCLUDE_DIR}")

# Finally the library itself
find_library(MagickCore_LIBRARY
        NAMES MagickCore
        PATHS ${MagickCore_PKGCONF_LIBRARY_DIRS}
        )
message(STATUS "    include path: ${MagickCore_LIBRARY}")

# Set the include dir variables and the libraries and let libfind_process do the rest.
# NOTE: Singular variables for this library, plural for libraries this this lib depends on.
#set(MagickCore_PROCESS_INCLUDES MagickCore_INCLUDE_DIR Magick_INCLUDE_DIRS)
#set(MagickCore_PROCESS_LIBS MagickCore_LIBRARY Magick_LIBRARIES)
set(MagickCore_PROCESS_INCLUDES MagickCore_INCLUDE_DIR)
set(MagickCore_PROCESS_LIBS MagickCore_LIBRARY)
libfind_process(MagickCore)
message(STATUS "    include path: ${MagickCore_INCLUDE_DIR}")
include_directories(${MagickCore_INCLUDE_DIR})


aux_source_directory(./src SOURCE_FILES)
#set(SOURCE_FILES src/ocr_read.cpp src/ocr_read.h src/ocr_main.cpp src/ocr_tesseract.cpp src/ocr_tesseract.h)

#message(STATUS "OpenCV library status:")
#message(STATUS "    version: ${OpenCV_VERSION}")
#message(STATUS "    libraries: ${OpenCV_LIBS}")
#message(STATUS "    include path: ${OpenCV_INCLUDE_DIRS}")
#message(STATUS "    include path: ${SOURCE_FILES}")

add_executable(ocr_detect ${SOURCE_FILES})
target_link_libraries(ocr_detect ${OpenCV_LIBS})
target_link_libraries(ocr_detect lept tesseract)
target_link_libraries(ocr_detect ${MagickCore_LIBRARY})

add_library(ocr_java SHARED ${SOURCE_FILES})
target_link_libraries(ocr_java ${OpenCV_LIBS})
target_link_libraries(ocr_java ${MagickCore_LIBRARY})
target_link_libraries(ocr_java lept tesseract)

