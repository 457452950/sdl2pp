#pragma once
#ifndef SDL2PP_BASE_SDLIMAGE_H_
#define SDL2PP_BASE_SDLIMAGE_H_

#include <memory>
#include <string_view>


#include <SDL_image.h>

namespace sdlpp {

class SDLSurface;

class SDLTexture;

class SDLRenderer;

std::shared_ptr<SDLSurface> IMG_LoadSurfaceFromFile(std::string_view path);

std::shared_ptr<SDLTexture> IMG_LoadTextureFromFile(std::string_view path, std::shared_ptr<SDLRenderer> renderer);


} // namespace sdlpp

#endif // SDL2PP_BASE_SDLIMAGE_H_
