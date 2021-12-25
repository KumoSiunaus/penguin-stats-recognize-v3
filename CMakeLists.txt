PROJECT(penguin-stats-recognize-v3)
cmake_minimum_required(VERSION 3.5)
set(CMAKE_CXX_STANDARD 17)

find_package(OpenCV REQUIRED)
include_directories(${OpenCV_INCLUDE_DIRS})
link_directories(${OpenCV_LIBRARY_DIRS})

add_library(penguin-stats-recognize-v3 SHARED penguin_wasm.cpp)
target_link_libraries(penguin-stats-recognize-v3 ${OpenCV_LIBS})