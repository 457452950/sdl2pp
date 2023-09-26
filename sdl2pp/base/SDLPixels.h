#pragma once
#ifndef SDL2PP_BASE_SDLPIXELS_H_
#define SDL2PP_BASE_SDLPIXELS_H_

#include <memory>

#include <SDL_pixels.h>

namespace sdlpp {

std::shared_ptr<SDL_PixelFormat> MakeShared(SDL_PixelFormat *&&pf);

std::unique_ptr<SDL_PixelFormat, decltype(&SDL_FreeFormat)> MakeUnique(SDL_PixelFormat *&&pf);

} // namespace sdlpp

#endif // SDL2PP_BASE_SDLPIXELS_H_
