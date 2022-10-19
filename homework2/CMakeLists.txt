cmake_minimum_required(VERSION 3.16.3)
project(homework2)

set(CMAKE_CXX_STANDARD 14)

find_package(OpenCV REQUIRED)
include_directories(${OpenCV_INCLUDE_DIRS})
include_directories("/usr/include/eigen3")
set(OpenGL_LIBS
        "/usr/lib/x86_64-linux-gnu/libGL.so"
        "/usr/lib/x86_64-linux-gnu/libGLU.so"
        "/usr/lib/x86_64-linux-gnu/libglut.so"
        )


add_executable(ex1 src/ex1/ex1.cpp)
target_link_libraries(ex1 ${OpenCV_LIBS})

add_executable(ex2 src/ex2/ex2.cpp)
target_link_libraries(ex2 ${OpenCV_LIBS})
target_link_libraries(ex2 ${OpenGL_LIBS})

add_executable(ex3 src/ex3/ex3.cpp)
target_link_libraries(ex3 ${OpenCV_LIBS})
target_link_libraries(ex3 ${OpenGL_LIBS})

add_executable(gl-test test/test.cpp)
target_link_libraries(gl-test ${OpenGL_LIBS})
