cmake_minimum_required(VERSION 2.8.12.2)
project(ocr)
set(OpenCV_DIR /usr/local)
include_directories(/usr/local/include)
include_directories(./)
include_directories(/usr/local/include/leptonica)
include_directories(/usr/lib/jvm/java-8-openjdk-amd64/include/)
find_package(OpenCV REQUIRED)
link_directories(/usr/local/lib)
set(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} -std=c++11")


aux_source_directory(./src SOURCE_FILES)

link_directories(./)
add_executable(ocr_detect ${SOURCE_FILES})
target_link_libraries(ocr_detect ${OpenCV_LIBS})


