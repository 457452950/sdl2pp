cmake_minimum_required(VERSION 3.26)
project(sdl2pp)

set(VCPKG_FEATURE_FLAGS "version")          # 用于支持自定义依赖版本
set(CMAKE_CXX_STANDARD 17)

find_package(SDL2 CONFIG REQUIRED)
if (SDL2_FOUND)
    message(STATUS "SDL2 found")
else ()
    message(FATAL_ERROR "SDL2 not found")
endif ()

find_package(SDL2_image CONFIG REQUIRED)
if (SDL2_image_FOUND)
    message(STATUS "SDL2_image found")
else ()
    message(FATAL_ERROR "SDL2_image not found")
endif ()

find_package(SDL2_ttf CONFIG REQUIRED)
if (SDL2_ttf_FOUND)
    message(STATUS "SDL2_ttf found")
else ()
    message(FATAL_ERROR "SDL2_ttf not found")
endif ()

find_package(SDL2_mixer CONFIG REQUIRED)
if (SDL2_mixer_FOUND)
    message(STATUS "SDL2_mixer found")
else ()
    message(FATAL_ERROR "SDL2_mixer not found")
endif ()

find_package(fmt CONFIG REQUIRED)


include_directories(
        ${SDL2_INCLUDE_DIRS}
        ${SDL2_IMAGE_INCLUDE_DIRS}
        ${SDL2_TTF_INCLUDE_DIRS}
        ${SDL2_MIXER_INCLUDE_DIRS}
        ${FMT_INCLUDE_DIRS}
        "./"
)


set(ENGINE_NAME "sdl2pp")

add_library(
        ${ENGINE_NAME}

        sdl2pp/base/SDLLog.h
        sdl2pp/base/Math.cpp
        sdl2pp/base/Math.h
        sdl2pp/base/Point.cpp
        sdl2pp/base/Point.h
        sdl2pp/base/Rect.cpp
        sdl2pp/base/Rect.h
        sdl2pp/base/Scale.cpp
        sdl2pp/base/Scale.h
        sdl2pp/base/SDLEnvironment.h
        sdl2pp/base/SDLEnvironment.cpp
        sdl2pp/base/SDLWindow.h
        sdl2pp/base/SDLWindow.cpp
        sdl2pp/base/SDLRenderer.h
        sdl2pp/base/SDLRenderer.cpp
        sdl2pp/base/SDLDisplay.h
        sdl2pp/base/SDLDisplay.cpp
        sdl2pp/base/SDLTexture.cpp
        sdl2pp/base/SDLTexture.h
        sdl2pp/base/SDLSurface.cpp
        sdl2pp/base/SDLSurface.h
        sdl2pp/base/SDLImage.cpp
        sdl2pp/base/SDLImage.h
        sdl2pp/base/SDLPixels.cpp
        sdl2pp/base/SDLPixels.h
        sdl2pp/base/SDLTTF.cpp
        sdl2pp/base/SDLTTF.h
        sdl2pp/base/Time.cpp
        sdl2pp/base/Time.h
        sdl2pp/base/SDLMixer.cpp
        sdl2pp/base/SDLMixer.h
        sdl2pp/base/SDLAudio.cpp
        sdl2pp/base/SDLAudio.h

        sdl2pp/SRenderer.cpp
        sdl2pp/SRenderer.h
        sdl2pp/STexture.cpp
        sdl2pp/STexture.h
        sdl2pp/SWindow.h
        sdl2pp/SWindow.cpp
        sdl2pp/SAnimation.cpp
        sdl2pp/SAnimation.h
        sdl2pp/Renderable.cpp
        sdl2pp/Renderable.h

        sdl2pp/SDLpp.h
)

target_link_libraries(
        ${ENGINE_NAME}
        $<TARGET_NAME_IF_EXISTS:SDL2::SDL2main>
        $<IF:$<TARGET_EXISTS:SDL2::SDL2>,SDL2::SDL2,SDL2::SDL2-static>
        $<IF:$<TARGET_EXISTS:SDL2_image::SDL2_image>,SDL2_image::SDL2_image,SDL2_image::SDL2_image-static>
        $<IF:$<TARGET_EXISTS:SDL2_ttf::SDL2_ttf>,SDL2_ttf::SDL2_ttf,SDL2_ttf::SDL2_ttf-static>
        $<IF:$<TARGET_EXISTS:SDL2_mixer::SDL2_mixer>,SDL2_mixer::SDL2_mixer,SDL2_mixer::SDL2_mixer-static>
        fmt::fmt
)


add_subdirectory(game)
