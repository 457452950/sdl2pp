#pragma once
#ifndef SDL2PP_SDL2PP_SDL_SDLPIXELS_H_
#define SDL2PP_SDL2PP_SDL_SDLPIXELS_H_

#include <memory>

#include <SDL_pixels.h>

namespace sdlpp {

std::shared_ptr<SDL_PixelFormat> MakeShared(SDL_PixelFormat *&&pf);

std::unique_ptr<SDL_PixelFormat, decltype(&SDL_FreeFormat)> MakeUnique(SDL_PixelFormat *&&pf);

using ColorData = uint32_t;

ColorData MapRGB(const SDL_PixelFormat *pf, SDL_Color color);

ColorData MapRGBA(const SDL_PixelFormat *pf, SDL_Color color);

SDL_Color GetRGB(ColorData pixel, const SDL_PixelFormat *pf);

SDL_Color GetRGBA(ColorData pixel, const SDL_PixelFormat *pf);

} // namespace sdlpp

#endif // SDL2PP_SDL2PP_SDL_SDLPIXELS_H_
