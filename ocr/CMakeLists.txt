cmake_minimum_required(VERSION 2.8.12.2)
project(ocr)
set(OpenCV_DIR /home/cloud/opencv3.0/opencv-3.0.0-alpha/release)
include_directories(/home/cloud/opencv_lib/include)
find_package(OpenCV REQUIRED)

set(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} -std=c++11")

set(SOURCE_FILES src/ocr_read.cpp)

message(STATUS "OpenCV library status:")
message(STATUS "    version: ${OpenCV_VERSION}")
message(STATUS "    libraries: ${OpenCV_LIBS}")
message(STATUS "    include path: ${OpenCV_INCLUDE_DIRS}")

add_executable(ocr_detect ${SOURCE_FILES})
target_link_libraries(ocr_detect ${OpenCV_LIBS})