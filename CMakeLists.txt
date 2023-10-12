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

        sdl2pp/common/SDLLog.h
        sdl2pp/common/Math.cpp
        sdl2pp/common/Math.h
        sdl2pp/common/Point.cpp
        sdl2pp/common/Point.h
        sdl2pp/common/Rect.cpp
        sdl2pp/common/Rect.h
        sdl2pp/common/Scale.cpp
        sdl2pp/common/Scale.h
        sdl2pp/common/SDLEnvironment.h
        sdl2pp/common/SDLEnvironment.cpp
        sdl2pp/common/SDLWindow.h
        sdl2pp/common/SDLWindow.cpp
        sdl2pp/common/SDLRenderer.h
        sdl2pp/common/SDLRenderer.cpp
        sdl2pp/common/SDLDisplay.h
        sdl2pp/common/SDLDisplay.cpp
        sdl2pp/common/SDLTexture.cpp
        sdl2pp/common/SDLTexture.h
        sdl2pp/common/SDLSurface.cpp
        sdl2pp/common/SDLSurface.h
        sdl2pp/common/SDLImage.cpp
        sdl2pp/common/SDLImage.h
        sdl2pp/common/SDLPixels.cpp
        sdl2pp/common/SDLPixels.h
        sdl2pp/common/SDLTTF.cpp
        sdl2pp/common/SDLTTF.h
        sdl2pp/common/Time.cpp
        sdl2pp/common/Time.h
        sdl2pp/common/SDLMixer.cpp
        sdl2pp/common/SDLMixer.h
        sdl2pp/common/SDLAudio.cpp
        sdl2pp/common/SDLAudio.h

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
