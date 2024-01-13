#include "SDLImage.h"

#include "SDLSurface.hpp"
#include "SDLTexture.hpp"
#include "SDLRenderer.hpp"

namespace sdlpp {
std::shared_ptr<SDLSurface> IMG_LoadSurfaceFromFile(std::string_view path) {
    return SDLSurface::Create(IMG_Load(path.data()));
}

std::shared_ptr<SDLTexture> IMG_LoadTextureFromFile(std::string_view path, std::shared_ptr<SDLRenderer> renderer) {
    return SDLTexture::Create(IMG_LoadTexture(renderer->Get(), path.data()));
    ;
}
} // namespace sdlpp