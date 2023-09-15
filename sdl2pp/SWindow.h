#pragma once
#ifndef SDL2PP_SDL2PP_SDL2PP_SWINDOW_H_
#define SDL2PP_SDL2PP_SDL2PP_SWINDOW_H_

#include <atomic>

#include <SDL_ttf.h>
#include <SDL_image.h>

#include "base/SDLWindow.h"


namespace sdlpp {

    inline SDL_Texture *loadTexture(const std::string &file, SDL_Renderer *ren) {
        SDL_Texture *texture = IMG_LoadTexture(ren, file.c_str());
        if (texture == nullptr) {
            SDL_LogError(0, "LoadTexture. %s %s", file.c_str(), SDL_GetError());
        }
        return texture;
    }

    inline void renderTexture(SDL_Texture *tex, SDL_Renderer *ren, SDL_Rect dst,
                              SDL_Rect *clip = nullptr) {
        SDL_RenderCopy(ren, tex, clip, &dst);
    }

/**
 * Draw an SDL_Texture to an SDL_Renderer at position x, y, preserving
 * the texture's width and height and taking a clip of the texture if desired
 * If a clip is passed, the clip's width and height will be used instead of
 *	the texture's
 * @param tex The source texture we want to draw
 * @param ren The renderer we want to draw to
 * @param x The x coordinate to draw to
 * @param y The y coordinate to draw to
 * @param clip The sub-section of the texture to draw (clipping rect)
 *		default of nullptr draws the entire texture
 */
    inline void renderTexture(SDL_Texture *tex, SDL_Renderer *ren, int x, int y,
                              SDL_Rect *clip = nullptr) {
        SDL_Rect dst;
        dst.x = x;
        dst.y = y;
        if (clip != nullptr) {
            dst.w = clip->w;
            dst.h = clip->h;
        } else {
            SDL_QueryTexture(tex, nullptr, nullptr, &dst.w, &dst.h);
        }
        renderTexture(tex, ren, dst, clip);
    }

/**
* RenderProcess the message we want to display to a texture for drawing
* @param message The message we want to display
* @param fontFile The font we want to use to render the text
* @param color The color we want the text to be
* @param fontSize The size we want the font to be
* @param renderer The renderer to load the texture in
* @return An SDL_Texture containing the rendered message, or nullptr if something went wrong
*/
    inline SDL_Texture *renderText(const std::string &message, const std::string &fontFile,
                                   SDL_Color color, int fontSize, SDL_Renderer *renderer) {
        //Open the font
        TTF_Font *font = TTF_OpenFont(fontFile.c_str(), fontSize);
        if (font == nullptr) {
            SDL_LogError(0, "ttf open font failed: %s", TTF_GetError());
            return nullptr;
        }
        //We need to first render to a surface as that's what TTF_RenderText
        //returns, then load that surface into a texture
        SDL_Surface *surf = TTF_RenderText_Blended(font, message.c_str(), color);
        if (surf == nullptr) {
            TTF_CloseFont(font);
            SDL_LogError(0, "ttf render font failed: %s", TTF_GetError());
            return nullptr;
        }
        SDL_Texture *texture = SDL_CreateTextureFromSurface(renderer, surf);
        if (texture == nullptr) {
            SDL_LogError(0, "create texture failed: %s", TTF_GetError());
        }
        //Clean up the surface and font
        SDL_FreeSurface(surf);
        TTF_CloseFont(font);
        return texture;
    }

    class SWindow : public SDLWindow {
        static const SDL_Point DEFAULT_SIZE;
    public:
        SWindow() : SDLWindow(DEFAULT_SIZE) {
            renderer_ = SDL_CreateRenderer(this->Get(), -1, 0);
            if (!renderer_) {
                SDL_LogError(SDL_LOG_CATEGORY_APPLICATION, "SDL_CreateRenderer failed. err: %s \n", SDL_GetError());
                SDL_assert(renderer_);
            }
        }

        ~SWindow() override {
            if (renderer_)
                SDL_DestroyRenderer(renderer_);
        }

        SDL_Renderer *GetRenderer() {
            return renderer_;
        }


        enum EventResult : int {
            Ignore,
            Continue,
        };

        virtual int WindowEvent(const SDL_WindowEvent &event) {
            return Continue;
        }

        void DefaultWindowEvent(const SDL_WindowEvent &event);

        virtual int KeyEvent(const SDL_KeyboardEvent &event) {
            return Ignore;
        }
        
        virtual void RenderProcess() {}

        void Close();

        int Exec();

    private:
        std::atomic_bool active_;
        SDL_Event event_{};
        SDL_Renderer *renderer_{nullptr};
    };

} // sdlpp

#endif //SDL2PP_SDL2PP_SDL2PP_SWINDOW_H_
