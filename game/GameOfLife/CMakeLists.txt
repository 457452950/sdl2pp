cmake_minimum_required(VERSION 3.26)
project(GOL)

set(CMAKE_CXX_STANDARD 17)

set(CMAKE_CXX_FLAGS "${CMAKE_C_FLAGS} -O1  -Wall")


set(ENGINE_NAME "sdl2pp")

add_executable(
        ${PROJECT_NAME}
        main.cpp
        Window.cpp
        Window.h
        GOLMap.cpp
        GOLMap.h
)

target_link_libraries(
        ${PROJECT_NAME}
        PRIVATE
        ${ENGINE_NAME}
)
