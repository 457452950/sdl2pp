cmake_minimum_required(VERSION 3.26)
project(sdlexample)

set(CMAKE_CXX_STANDARD 17)

set(ENGINE_NAME "sdl2pp")

add_executable(
        sdlexample
        main.cpp
        Window.cpp
        Window.h
)

target_link_libraries(
        sdlexample
        PRIVATE
        ${ENGINE_NAME}
)
