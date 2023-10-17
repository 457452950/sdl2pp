cmake_minimum_required(VERSION 3.26)
project(pong)

set(CMAKE_CXX_STANDARD 17)

set(ENGINE_NAME "sdl2pp")

add_executable(
        ${PROJECT_NAME}
        main.cpp
        Window.cpp
        Window.h
        Ball.cpp
        Ball.h
)

target_link_libraries(
        ${PROJECT_NAME}
        PRIVATE
        ${ENGINE_NAME}
)
