cmake_minimum_required(VERSION 3.26)
project(sdlgl)

set(CMAKE_CXX_STANDARD 17)

add_executable(
        ${PROJECT_NAME}
        main.cpp
        Window.cpp
        Window.h
        gl/GLObject.cpp
        gl/GLObject.hpp
        gl/Mesh.cpp
        gl/Mesh.hpp
        gl/Shader.cpp
        gl/Shader.hpp
        gl/Model.cpp
        gl/Model.hpp
)

target_link_libraries(
        ${PROJECT_NAME}
        PRIVATE
        ${ENGINE_NAME}
        glm::glm
        assimp::assimp
)
