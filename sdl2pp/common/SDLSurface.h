#pragma once
#ifndef SDL2PP_BASE_SDLSURFACE_H_
#define SDL2PP_BASE_SDLSURFACE_H_

#include <memory>
#include <tuple>

#include <SDL_surface.h>
#include <SDL_image.h>

#include "SDLRenderer.h"
#include "SDLLog.h"
#include "SDLPixels.h"
#include "Point.h"
#include "shape/Rect.h"

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
        auto color_key = MapRGB(surface_->format, color);
        return SDL_SetColorKey(surface_, is_set ? SDL_TRUE : SDL_FALSE, color_key) == 0;
    }
    bool HasColorKey() { return SDL_HasColorKey(this->surface_); }
    bool GetColorKey(SDL_Color &color) {
        ColorData c{};
        auto      res = SDL_GetColorKey(this->surface_, &c);
        if(res != 0) {
            LOG_ERR(log::LIB, "get color key error : {}", SDL_GetError());
            return false;
        }

        color = GetRGB(c, this->surface_->format);
        return true;
    }

    bool MustLock() { return SDL_MUSTLOCK(this->surface_); }
    bool Lock() { return SDL_LockSurface(this->surface_) == 0; }
    void Unlock() { SDL_UnlockSurface(this->surface_); }

    std::shared_ptr<SDLRenderer> CreateSoftwareRenderer() {
        return SDLRenderer::Create(SDL_CreateSoftwareRenderer(this->surface_));
    }

    // 翻转
    std::shared_ptr<SDLSurface> Flip(int flip_flags) { // 指向将要翻转的表面的指针
        std::shared_ptr<SDLSurface> flipped;

        if(HasColorKey()) {
            flipped = Create(SDL_CreateRGBSurface(SDL_SWSURFACE,
                                                  surface_->w,
                                                  surface_->h,
                                                  surface_->format->BitsPerPixel,
                                                  surface_->format->Rmask,
                                                  surface_->format->Gmask,
                                                  surface_->format->Bmask,
                                                  0));
        } else {
            flipped = Create(SDL_CreateRGBSurface(SDL_SWSURFACE,
                                                  surface_->w,
                                                  surface_->h,
                                                  surface_->format->BitsPerPixel,
                                                  surface_->format->Rmask,
                                                  surface_->format->Gmask,
                                                  surface_->format->Bmask,
                                                  surface_->format->Amask));
        }

        if(this->MustLock()) {
            this->Lock();
        }
        for(int x = 0, rx = this->surface_->w - 1; x < this->surface_->w; x++, rx--) {
            // 遍历行
            for(int y = 0, ry = this->surface_->h - 1; y < this->surface_->h; y++, ry--) {
                switch(surface_->format->BitsPerPixel) {
                case 32: {
                    auto pixel = _GetPixel<uint32_t>(x, y);

                    // 复制像素
                    if((flip_flags & Flip::SDL_FLIP_VERTICAL) && (flip_flags & Flip::SDL_FLIP_HORIZONTAL)) {
                        flipped->_SetPixel(rx, ry, pixel);
                    } else if(flip_flags & Flip::SDL_FLIP_HORIZONTAL) {
                        flipped->_SetPixel(rx, y, pixel);
                    } else if(flip_flags & Flip::SDL_FLIP_VERTICAL) {
                        flipped->_SetPixel(x, ry, pixel);
                    }
                } break;
                default:
                    SDL_assert(false);
                }
            }
        }

        if(this->MustLock()) {
            this->Unlock();
        }

        if(this->HasColorKey()) {
            SDL_Color color{};
            auto      ok = this->GetColorKey(color);
            if(!ok) {
                LOG_ERR(log::LIB, "get color key error : {}", SDL_GetError());
                return nullptr;
            }

            ok = flipped->SetColorKey(true, color);
            if(!ok) {
                LOG_ERR(log::LIB, "set color key error : {}", SDL_GetError());
                return nullptr;
            }
        }

        return flipped;
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
    template <class T>
    T _GetPixel(int x, int y) {
        T *pixels = static_cast<T *>(this->surface_->pixels);
        return pixels[y * this->surface_->w + x];
    }
    template <class T>
    void _SetPixel(int x, int y, T pixel) {
        T *pixels = static_cast<T *>(this->surface_->pixels);

        pixels[y * this->surface_->w + x] = pixel;
    }

private:
    explicit SDLSurface(SDL_Surface *&&surface) : surface_(std::move(surface)) {}

    SDL_Surface *surface_{nullptr};
};

} // namespace sdlpp

#endif // SDL2PP_BASE_SDLSURFACE_H_
