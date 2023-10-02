cmake_minimum_required(VERSION 3.26)
project(example)

set(CMAKE_CXX_STANDARD 17)

#include_directories(
#        ${SDL2_INCLUDE_DIRS}
#        ${SDL2_IMAGE_INCLUDE_DIRS}
#        ${SDL2_TTF_INCLUDE_DIRS}
#        ${SDL2_MIXER_INCLUDE_DIRS}
#        ${FMT_INCLUDE_DIRS}
#        ${PROJECT_SOURCE_DIR}
#)


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
