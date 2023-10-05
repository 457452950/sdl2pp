cmake_minimum_required(VERSION 3.26)
project(snake)

set(CMAKE_CXX_STANDARD 17)

set(ENGINE_NAME "sdl2pp")

add_executable(
        snake
        main.cpp

        Window.cpp
        Window.h

        tools/Resource.cpp
        tools/Resource.h

        ui/UI.cpp
        ui/UI.h
        snake/Game.cpp
        snake/Game.h
)

target_link_libraries(
        snake
        PRIVATE
        ${ENGINE_NAME}
)
