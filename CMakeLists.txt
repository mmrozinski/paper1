cmake_minimum_required(VERSION 3.26)
project(paper1)

set(CMAKE_CXX_STANDARD 17)

add_executable(paper1 main.cpp
        Camera.cpp
        Camera.h
        Vector3.cpp
        Vector3.h
        Matrix4.cpp
        Matrix4.h
)

find_package(OpenGL REQUIRED)
find_package(GLUT REQUIRED)

target_link_libraries(paper1 -lOpenGL32 -lfreeGLUT)
