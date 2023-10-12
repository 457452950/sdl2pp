#pragma once
#ifndef SDL2PP_BASE_SDLSURFACE_H_
#define SDL2PP_BASE_SDLSURFACE_H_

#include <memory>
#include <tuple>

#include <SDL_surface.h>
#include <SDL_image.h>

#include "Point.h"
#include "Rect.h"

namespace sdlpp {

std::shared_ptr<SDL_Surface> MakeShared(SDL_Surface *&&surface);

auto MakeUnique(SDL_Surface *&&surface) -> std::unique_ptr<SDL_Surface, decltype(&SDL_FreeSurface)>;

class SDLSurface;


// Copy an existing surface to a new surface of the specified format.
std::shared_ptr<SDLSurface> ConvertSurface(std::shared_ptr<SDLSurface> src, const SDL_PixelFormat &fmt);
// Copy an existing surface to a new surface of the specified format.
std::shared_ptr<SDLSurface> ConvertSurface(std::shared_ptr<SDLSurface> src, uint32_t pixel_format_enum);

class SDLSurface {
public:
    static std::shared_ptr<SDLSurface> Create(SDL_Surface *&&surface);

    const SDL_Surface *Get() const { return surface_; }

    SDL_Surface *Get() { return surface_; }

    uint32_t GetFlags() { return surface_->flags; }

    const SDL_PixelFormat *GetFormat() const { return surface_->format; }

    int GetHeight() const { return surface_->h; }

    int GetWidth() const { return surface_->w; }

    std::tuple<int, int> GetSize() const { return std::make_tuple(surface_->w, surface_->h); }

    int GetPitch() const { return surface_->pitch; }

    const void *GetPixels() const { return surface_->pixels; }

    void *GetPixels() { return surface_->pixels; }

    bool IsValid() const { return surface_ != nullptr; }

    std::shared_ptr<SDLSurface> Copy();

    bool BlitCopy(std::shared_ptr<SDLSurface> dst, const RectI *src_rect = nullptr, const PointI &dst_point = {0, 0});

    /**
     * Use this function to perform a scaled surface copy to a destination surface.
     * @param dst      the SDL_Surface structure that is the blit target
     * @param src_rect the SDL_Rect structure representing the rectangle to be copied,or NULL to copy the entire surface
     * @param dst_rect the SDL_Rect structure representing the rectangle that is copied into, or NULL to copy into the
     *                  entire surface
     * @return
     */
    bool BlitScaled(std::shared_ptr<SDLSurface> dst, const RectI *src_rect = nullptr, RectI *dst_rect = nullptr);

    /**
     * 设置指定颜色透明，多次调用覆盖，不叠加
     * @param is_set 是否开启
     * @param color 忽略 alpha
     * @return
     */
    bool SetColorKey(bool is_set, SDL_Color color) {
        auto color_key = SDL_MapRGB(surface_->format, color.r, color.g, color.b);
        return SDL_SetColorKey(surface_, is_set ? SDL_TRUE : SDL_FALSE, color_key) == 0;
    }

    ~SDLSurface() noexcept {
        if(surface_) {
            SDL_FreeSurface(surface_);
            surface_ = nullptr;
        }
    }

    // no copy
    SDLSurface(const SDLSurface &) = delete;

    SDLSurface &operator=(const SDLSurface &) = delete;

    // moving
    SDLSurface(SDLSurface &&) = default;

    SDLSurface &operator=(SDLSurface &&) = default;

private:
    explicit SDLSurface(SDL_Surface *&&surface) : surface_(std::move(surface)) {}

    SDL_Surface *surface_{nullptr};
};

} // namespace sdlpp

#endif // SDL2PP_BASE_SDLSURFACE_H_
