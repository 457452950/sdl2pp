#pragma  once
#ifndef SDL2PP_SDL2PP_BASE_SDLTEXTURE_H_
#define SDL2PP_SDL2PP_BASE_SDLTEXTURE_H_

#include <memory>
#include <tuple>

#include <SDL_render.h>

namespace sdlpp {

    inline std::shared_ptr<SDL_Texture> MakeShared(SDL_Texture *&&texture) {
        return {texture, SDL_DestroyTexture};
    }

    inline std::unique_ptr<SDL_Texture, decltype(&SDL_DestroyTexture)> MakeUnique(SDL_Texture *&&texture) {
        return std::unique_ptr<SDL_Texture, decltype(&SDL_DestroyTexture)>{texture, SDL_DestroyTexture};
    }

    class SDLTexture {
    public:
        using Access = SDL_TextureAccess;
        using Modulate = SDL_TextureModulate;
        using BlendMode = SDL_BlendMode;
        using ScaleMode = SDL_ScaleMode;


        bool Query(uint32_t *format, int *access, int *w, int *h) {
            return SDL_QueryTexture(texture_, format, access, w, h) == 0;
        }

        // <w, h>
        std::tuple<int, int> GetSize() {
            int w, h;
            if (SDL_QueryTexture(texture_, nullptr, nullptr, &w, &h) == 0)
                return std::make_tuple(w, h);

            return std::make_tuple(-1, -1);
        }

        // can be null
        bool GetSize(int *w, int *h) {
            return SDL_QueryTexture(texture_, nullptr, nullptr, w, h) == 0;
        }

        int GetWidth() {
            int w;
            if (SDL_QueryTexture(texture_, nullptr, nullptr, &w, nullptr) == 0)
                return w;

            return -1;
        }

        int GetHeight() {
            int h;
            if (SDL_QueryTexture(texture_, nullptr, nullptr, nullptr, &h) == 0)
                return h;

            return -1;
        }

        // ignore color.a
        bool SetColorMod(SDL_Color rgb) {
            return SDL_SetTextureColorMod(texture_, rgb.r, rgb.g, rgb.b) == 0;
        }

        bool GetColorMod(SDL_Color &rgb) {
            return SDL_GetTextureColorMod(texture_, &rgb.r, &rgb.g, &rgb.b) == 0;
        }

        bool SetAlphaMod(uint8_t alpha) {
            return SDL_SetTextureAlphaMod(texture_, alpha) == 0;
        }

        bool GetAlphaMod(uint8_t &alpha) {
            return SDL_GetTextureAlphaMod(texture_, &alpha) == 0;
        }

        bool SetBlendMode(BlendMode blend) {
            return SDL_SetTextureBlendMode(texture_, blend) == 0;
        }

        bool GetBlendMode(BlendMode &blend) {
            return SDL_GetTextureBlendMode(texture_, &blend) == 0;
        }

        bool SetScaleMode(ScaleMode scale) {
            return SDL_SetTextureScaleMode(texture_, scale) == 0;
        }

        bool GetScaleMode(ScaleMode &scale) {
            return SDL_GetTextureScaleMode(texture_, &scale) == 0;
        }

        bool SetUserData(void *userdata) {
            return SDL_SetTextureUserData(texture_, userdata) == 0;
        }

        void *GetUserData() {
            return SDL_GetTextureUserData(texture_);
        }

        // SDL_UpdateTexture
        // SDL_UpdateYUVTexture
        // SDL_UpdateNVTexture
        // SDL_LockTexture
        // SDL_LockTextureToSurface
        // SDL_UnlockTexture




        static std::shared_ptr<SDLTexture> Create(SDL_Texture *&&texture) {
            return std::shared_ptr<SDLTexture>(new SDLTexture(std::move(texture)));
        }

        SDL_Texture *Get() {
            return texture_;
        }

        const SDL_Texture *Get() const {
            return texture_;
        }

        bool IsValid() {
            return texture_ != nullptr;
        }

        ~SDLTexture() {
            if (texture_) {
                SDL_DestroyTexture(texture_);
            }
        }

        SDLTexture(const SDLTexture &other) = delete;

        SDLTexture &operator=(const SDLTexture &other) = delete;

        SDLTexture(SDLTexture &&other) = default;

        SDLTexture &operator=(SDLTexture &&other) = default;

    private:
        SDLTexture(SDL_Texture *&&texture) : texture_(texture) {}

        SDL_Texture *texture_;
    };

} // sdlpp

#endif //SDL2PP_SDL2PP_BASE_SDLTEXTURE_H_
