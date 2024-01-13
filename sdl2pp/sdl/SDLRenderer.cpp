#include "SDLRenderer.hpp"

#include "SDLWindow.h"
#include "SDLTexture.hpp"
#include "SDLSurface.hpp"

namespace sdlpp {

RenderDrivers::RenderDrivers() {
    auto count = SDL_GetNumRenderDrivers();
    this->renderers_info_.resize(count);

    for(int i = 0; i < count; ++i) {
        SDL_GetRenderDriverInfo(i, &this->renderers_info_[i]);
    }
}

std::shared_ptr<SDLRenderer> SDLRenderer::Create(std::shared_ptr<SDLWindow> window, Uint32 flags, int index) {
    return Create(SDL_CreateRenderer(window->Get(), index, flags));
}

std::shared_ptr<SDLTexture> SDLRenderer::CreateTextureFromSurface(std::shared_ptr<SDLSurface> surface) {
    return SDLTexture::Create(SDL_CreateTextureFromSurface(this->renderer_, surface->Get()));
}

bool SDLRenderer::SetRenderTarget(std::shared_ptr<SDLTexture> texture) {
    return SDL_SetRenderTarget(this->renderer_, texture->Get()) == 0;
}

bool SDLRenderer::SetRenderTarget(SDL_Texture *texture) { return SDL_SetRenderTarget(this->renderer_, texture) == 0; }

bool SDLRenderer::Update(std::shared_ptr<SDLTexture> texture, const RectI *src_rect, const RectI *dst_rect) {
    return SDL_RenderCopy(renderer_, texture->Get(), src_rect, dst_rect) == 0;
}

bool SDLRenderer::Update(std::shared_ptr<SDLTexture> texture, const RectI *src_rect, const RectF *dst_rect) {
    return SDL_RenderCopyF(renderer_, texture->Get(), src_rect, dst_rect) == 0;
}

bool SDLRenderer::Update(std::shared_ptr<SDLTexture> texture,
                         const RectI                *src_rect,
                         const RectI                *dst_rect,
                         double                      angle,
                         const PointI               &center,
                         Flip                        flip) {
    return SDL_RenderCopyEx(renderer_, texture->Get(), src_rect, dst_rect, angle, &center, flip) == 0;
}

bool SDLRenderer::Update(std::shared_ptr<SDLTexture> texture,
                         const RectI                *src_rect,
                         const RectF                *dst_rect,
                         double                      angle,
                         const PointF               &center,
                         Flip                        flip) {
    return SDL_RenderCopyExF(renderer_, texture->Get(), src_rect, dst_rect, angle, &center, flip) == 0;
}

} // namespace sdlpp