cmake_minimum_required(VERSION 3.26)
project(paper1)

set(CMAKE_CXX_STANDARD 23)

add_executable(paper1 main.cpp
        Camera.cpp
        Camera.h
        Vector3.cpp
        Vector3.h
        Matrix4.cpp
        Matrix4.h
        MeshRenderer.cpp
        MeshRenderer.h
        Shader.cpp
        Shader.h
        FileHelper.h
        ChunkRenderer.cpp
        ChunkRenderer.h
        Chunk.cpp
        Chunk.h
        Block.h
        Vector3i.cpp
        Vector3i.h
)

find_package(OpenGL REQUIRED)
find_package(GLUT REQUIRED)
find_package(GLEW REQUIRED)

target_link_libraries(paper1 -lOpenGL32 -lfreeGLUT GLEW::GLEW)
