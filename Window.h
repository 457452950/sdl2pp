#pragma once
#ifndef SDL2PP_GAME_WINDOW_H_
#define SDL2PP_GAME_WINDOW_H_

#include <fmt/format.h>

#include "sdl2pp/SDLpp.h"
#include "sdl2pp/STexture.h"

namespace game {

class Window : public sdlpp::SWindow {
public:
    Window();

    ~Window() override {}

    int KeyEvent(const SDL_KeyboardEvent &event) override {
        SDL_Log("scan key = %d %s", event.keysym.scancode, SDL_GetScancodeName(event.keysym.scancode));

        if(event.state != SDL_RELEASED) {
            return SWindow::KeyEvent(event);
        }

        switch(event.keysym.sym) {
        case SDLK_ESCAPE:
        case SDLK_q: {
            this->Close();
            break;
        }
        case SDLK_e: {
            break;
        }
        case SDLK_a: {
            break;
        }
        case SDLK_d: {
            break;
        }
        case SDLK_w: {
            break;
        }
        case SDLK_s: {
            break;
        }
        default:
            SDL_Log("key = %d %s", event.keysym.sym, SDL_GetKeyName(event.keysym.sym));
            break;
        }
        return SWindow::KeyEvent(event);
    }

    int WindowEvent(const SDL_WindowEvent &event) override {
        switch(event.event) {
        case SDL_WINDOWEVENT_RESIZED: {
            SDL_Log("resize %d %d ", event.data1, event.data2);
            break;
        }
        case SDL_WINDOWEVENT_CLOSE: {
            SDL_LogDebug(0, "SDL_WINDOW EVENT close but no close");
        }
        }
        return SWindow::WindowEvent(event);
    }

    int MouseButtonEvent(const SDL_MouseButtonEvent &event) override {
        SDL_Log("mouse button state %d x %d y %d button %d click %d",
                event.state,
                event.x,
                event.y,
                event.button,
                event.clicks);
        return SWindow::MouseButtonEvent(event);
    }

    int MouseMoveEvent(const SDL_MouseMotionEvent &event) override {
        //            SDL_Log("mouse motion state %d x %d y %d  xrel %d yrel %d",
        //                    event.state, event.x, event.y, event.xrel, event.yrel);
        return SWindow::MouseMoveEvent(event);
    }

    int MouseWheelEvent(const SDL_MouseWheelEvent &event) override {
        SDL_Log("mouse wheel state %d x %d y %d direction %d", event.windowID, event.x, event.y, event.direction);
        return SWindow::MouseWheelEvent(event);
    }

    void RenderProcess(SDL_FPoint view_pos, double view_angle) override;


private:
    std::shared_ptr<sdlpp::STexture> texture_man_;
    std::shared_ptr<sdlpp::STexture> texture_image_;
    std::shared_ptr<sdlpp::STexture> texture_image2_;

    uint16_t mix = 255;

    std::vector<sdlpp::SDLTexture::BlendMode> values = {
            sdlpp::SDLTexture::BlendMode::SDL_BLENDMODE_NONE,
            sdlpp::SDLTexture::BlendMode::SDL_BLENDMODE_BLEND,
            sdlpp::SDLTexture::BlendMode::SDL_BLENDMODE_ADD,
            sdlpp::SDLTexture::BlendMode::SDL_BLENDMODE_MOD,
            sdlpp::SDLTexture::BlendMode::SDL_BLENDMODE_MUL,
    };
    int index = 1;

    double angle_{0};

    std::shared_ptr<sdlpp::Font> font_;
};

} // namespace game

#endif // SDL2PP_GAME_WINDOW_H_
