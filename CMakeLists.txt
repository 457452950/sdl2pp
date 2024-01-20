cmake_minimum_required(VERSION 3.20)
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

find_package(Eigen3 CONFIG REQUIRED)
find_package(fmt CONFIG REQUIRED)
#find_package(imgui CONFIG REQUIRED)
find_package(glm CONFIG REQUIRED)
find_package(assimp CONFIG REQUIRED)

add_subdirectory(lazy_box)
include_directories("lazy_box/include")
link_directories("lazy_box/lib")

include_directories(
        ${SDL2_INCLUDE_DIRS}
        ${SDL2_IMAGE_INCLUDE_DIRS}
        ${SDL2_TTF_INCLUDE_DIRS}
        ${SDL2_MIXER_INCLUDE_DIRS}
        ${FMT_INCLUDE_DIRS}
        "./"
)
include_directories("thirdparty/glad/include")
include_directories("thirdparty/imgui/include")

set(GLAD_SOURCE "thirdparty/glad/src/glad.c")
set(IMGUI_SOURCE
        "thirdparty/imgui/src/imgui.cpp"
        "thirdparty/imgui/src/imgui_draw.cpp"
        "thirdparty/imgui/src/imgui_tables.cpp"
        "thirdparty/imgui/src/imgui_widgets.cpp"
        "thirdparty/imgui/src/imgui_impl_opengl3.cpp"
        "thirdparty/imgui/src/imgui_impl_sdl2.cpp"
        "thirdparty/imgui/src/imgui_impl_sdlrenderer2.cpp")

set(ENGINE_NAME ${PROJECT_NAME})

add_library(
        ${ENGINE_NAME}

        ${GLAD_SOURCE}
        ${IMGUI_SOURCE}

        sdl2pp/common/SDLLog.h
        sdl2pp/common/Math.cpp
        sdl2pp/common/Math.h
        sdl2pp/common/Point.cpp
        sdl2pp/common/Point.hpp
        sdl2pp/common/Scale.cpp
        sdl2pp/common/Scale.hpp
        sdl2pp/common/Time.cpp
        sdl2pp/common/Time.hpp

        sdl2pp/common/shape/Rect.cpp
        sdl2pp/common/shape/Rect.h
        sdl2pp/common/shape/Circle.cpp
        sdl2pp/common/shape/Circle.h

        sdl2pp/sdl/SDLEnvironment.hpp
        sdl2pp/sdl/SDLEnvironment.cpp
        sdl2pp/sdl/SDLWindow.hpp
        sdl2pp/sdl/SDLWindow.cpp
        sdl2pp/sdl/SDLRenderer.hpp
        sdl2pp/sdl/SDLRenderer.cpp
        sdl2pp/sdl/SDLDisplay.hpp
        sdl2pp/sdl/SDLDisplay.cpp
        sdl2pp/sdl/SDLTexture.cpp
        sdl2pp/sdl/SDLTexture.hpp
        sdl2pp/sdl/SDLSurface.cpp
        sdl2pp/sdl/SDLSurface.hpp
        sdl2pp/sdl/SDLImage.cpp
        sdl2pp/sdl/SDLImage.h
        sdl2pp/sdl/SDLPixels.cpp
        sdl2pp/sdl/SDLPixels.h
        sdl2pp/sdl/SDLTTF.cpp
        sdl2pp/sdl/SDLTTF.hpp
        sdl2pp/sdl/SDLMixer.cpp
        sdl2pp/sdl/SDLMixer.hpp
        sdl2pp/sdl/SDLAudio.cpp
        sdl2pp/sdl/SDLAudio.hpp

        sdl2pp/gl/GL.cpp
        sdl2pp/gl/GL.hpp

        sdl2pp/SRenderer.cpp
        sdl2pp/SRenderer.hpp
        sdl2pp/STexture.cpp
        sdl2pp/STexture.hpp
        sdl2pp/SWindow.hpp
        sdl2pp/SWindow.cpp
        sdl2pp/SAnimation.cpp
        sdl2pp/SAnimation.hpp
        sdl2pp/Renderable.cpp
        sdl2pp/Renderable.hpp
        sdl2pp/GLWindow.cpp

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
        Eigen3::Eigen
        lbox
)


add_subdirectory(game)
