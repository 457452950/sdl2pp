#pragma once
#ifndef SDL2PP_SDL2PP_GL_GL_HPP_
#define SDL2PP_SDL2PP_GL_GL_HPP_

#include <glad/glad.h>
#include <SDL_opengl.h>
#include <SDL_video.h>

#include "Toy/NonCopyAble.hpp"

namespace sdlpp::gl {

bool Init(int major, int minor);

bool SetDoubleBuffer(bool v);

int GetSwapInterval();

bool SetSwapInterval(int v);

bool CreateContext(SDL_Window *window);

void DestroyContext();

bool MakeCurrent(SDL_Window *window);

SDL_Window *GetCurrentWindow();

namespace ext {

// glad init
void Init();

void DepthTest();

} // namespace ext

} // namespace sdlpp::gl

#endif // SDL2PP_SDL2PP_GL_GL_HPP_
