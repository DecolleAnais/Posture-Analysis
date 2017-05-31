cmake_minimum_required(VERSION 3.6)
project(Posture_Analysis)

set(CMAKE_CXX_STANDARD 14)

set(SOURCE_FILES main.cpp BackgroundExtractor.cpp BackgroundExtractor.h SkinBlobExtractor.cpp SkinBlobExtractor.h)
add_executable(Posture_Analysis ${SOURCE_FILES})

# include opencv
find_package(OpenCV REQUIRED)
include_directories(${OpenCV_INCLUDE_DIRS})
target_link_libraries(Posture_Analysis ${OpenCV_LIBS})