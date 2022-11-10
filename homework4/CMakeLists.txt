cmake_minimum_required(VERSION 3.16)
project(homework4)

set(CMAKE_CXX_STANDARD 14)

set(SOURCE_FILES ex1 ex3 ex4)

find_package(OpenCV REQUIRED)
find_package(Eigen3 REQUIRED)

include_directories(${OpenCV_INCLUDE_DIRS})
include_directories(${EIGEN3_INCLUDE_DIR})

foreach (source_file ${SOURCE_FILES})
    add_executable(${source_file} src/${source_file}/${source_file}.cpp)
    target_link_libraries(${source_file} ${OpenCV_LIBS})
endforeach ()

add_executable(ex2 src/ex2/ex2.cpp src/ex2/VO.cpp src/ex2/VO.h)
target_link_libraries(ex2 ${OpenCV_LIBS})

