cmake_minimum_required(VERSION 3.26)
project(example)

set(CMAKE_CXX_STANDARD 17)

set(ENGINE_NAME "sdl2pp")

add_executable(
        example
        main.cpp
        Window.cpp
        Window.h
)

target_link_libraries(
        example
        PRIVATE
        ${ENGINE_NAME}
)
