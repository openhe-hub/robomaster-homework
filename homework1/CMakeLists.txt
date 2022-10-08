cmake_minimum_required(VERSION 3.16)
project(homework1)

set(CMAKE_CXX_STANDARD 14)

find_package(OpenCV REQUIRED)

add_executable(homework1 main.cpp)

add_executable(ex2 src/ex2/ex2.cpp)
target_link_libraries(ex2 ${OpenCV_LIBS})

add_executable(ex3 src/ex3/ex3.cpp)
target_link_libraries(ex3 ${OpenCV_LIBS})