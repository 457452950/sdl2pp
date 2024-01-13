#pragma once
#ifndef SDL2PP_GAME_WINDOW_H_
#define SDL2PP_GAME_WINDOW_H_

#include "sdl2pp/SDLpp.h"
#include "sdl2pp/STexture.h"


class Window : public sdlpp::SWindow {
public:
    Window();

    ~Window() override;

    int KeyEvent(const SDL_KeyboardEvent &event) override {
        LOG_VER(log::APP,
                "scan key = {} {}",
                static_cast<int>(event.keysym.scancode),
                SDL_GetScancodeName(event.keysym.scancode));

        if(event.state != SDL_RELEASED) {
            return SWindow::KeyEvent(event);
        }

        switch(event.keysym.sym) {
        case SDLK_ESCAPE: {
            this->Close();
            break;
        }
        case SDLK_q: {
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
            LOG_DBG(log::APP, "key = {} {}", event.keysym.sym, SDL_GetKeyName(event.keysym.sym));
            break;
        }
        return SWindow::KeyEvent(event);
    }

    int WindowEvent(const SDL_WindowEvent &event) override;

    int MouseButtonEvent(const SDL_MouseButtonEvent &event) override { return SWindow::MouseButtonEvent(event); }

    int MouseMoveEvent(const SDL_MouseMotionEvent &event) override { return SWindow::MouseMoveEvent(event); }

    int MouseWheelEvent(const SDL_MouseWheelEvent &event) override { return SWindow::MouseWheelEvent(event); }

    void RenderProcess() override;
    void RenderFlush() override;

private:
};

#endif // SDL2PP_GAME_WINDOW_H_
