#pragma once
#ifndef SDL2PP_SDL2PP_BASE_SDLSURFACE_H_
#define SDL2PP_SDL2PP_BASE_SDLSURFACE_H_

#include <memory>

#include <SDL_surface.h>
#include <SDL_image.h>

namespace sdlpp {

    std::shared_ptr<SDL_Surface> MakeShared(SDL_Surface *&&surface);

    auto MakeUnique(SDL_Surface *&&surface) -> std::unique_ptr<SDL_Surface, decltype(&SDL_FreeSurface)>;

    class SDLSurface {
    public:

        static std::shared_ptr<SDLSurface> Create(SDL_Surface *&&surface);

        const SDL_Surface *Get() const { return surface_; }

        SDL_Surface *Get() { return surface_; }

        uint32_t GetFlags() {
            return surface_->flags;
        }

        const SDL_PixelFormat *GetFormat() const {
            return surface_->format;
        }

        int GetHeight() const {
            return surface_->h;
        }

        int GetWidth() const {
            return surface_->w;
        }

        int GetPitch() const {
            return surface_->pitch;
        }

        const void *GetPixels() const {
            return surface_->pixels;
        }

        void *GetPixels() {
            return surface_->pixels;
        }

        bool IsValid() const { return surface_ != nullptr; }

        std::shared_ptr<SDLSurface> Copy();

        ~SDLSurface() noexcept {
            if (surface_) {
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

} // sdlpp

#endif //SDL2PP_SDL2PP_BASE_SDLSURFACE_H_
