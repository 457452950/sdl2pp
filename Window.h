#pragma once
#ifndef SDL2PP__WINDOW_H_
#define SDL2PP__WINDOW_H_

#include <fmt/format.h>

#include "sdl2pp/SDLpp.h"

namespace game {

    class Window : public sdlpp::SWindow {
    public:
        Window();

        ~Window() override {
        }

        int KeyEvent(const SDL_KeyboardEvent &event) override {
            SDL_Log("scan key = %d %s", event.keysym.scancode,
                    SDL_GetScancodeName(event.keysym.scancode));
            switch (event.keysym.sym) {
                case SDLK_ESCAPE:
                case SDLK_q: {
                    this->Close();
                    break;
                }
                case SDLK_e : {
                    boolen = !boolen;
                    SDL_Log("boolen = %d", boolen);
                    this->SetBordered(boolen);
                    break;
                }
                case SDLK_a: {
                    brightness -= 0.1f;
                    SDL_Log("brightness = %f", brightness);
                    this->SetBrightness(brightness);
                    break;
                }
                case SDLK_d: {
                    brightness += 0.1f;
                    SDL_Log("brightness = %f", brightness);
                    this->SetBrightness(brightness);
                    break;
                }
                case SDLK_w: {
                    this->SetFullscreen(true);
                    break;
                }
                case SDLK_s: {
                    this->SetFullscreen(false);
                    break;
                }
                default:
                    SDL_Log("key = %d %s", event.keysym.sym, SDL_GetKeyName(event.keysym.sym));
                    break;
            }
            return SWindow::KeyEvent(event);
        }

        int WindowEvent(const SDL_WindowEvent &event) override {
            switch (event.event) {
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
                    event.state, event.x, event.y, event.button, event.clicks);
            return SWindow::MouseButtonEvent(event);
        }

        int MouseMoveEvent(const SDL_MouseMotionEvent &event) override {
//            SDL_Log("mouse motion state %d x %d y %d  xrel %d yrel %d",
//                    event.state, event.x, event.y, event.xrel, event.yrel);
            return SWindow::MouseMoveEvent(event);
        }

        int MouseWheelEvent(const SDL_MouseWheelEvent &event) override {
            SDL_Log("mouse wheel state %d x %d y %d direction %d",
                    event.windowID, event.x, event.y, event.direction);
            return SWindow::MouseWheelEvent(event);
        }

        void RenderProcess() override;


    private:
        std::shared_ptr<sdlpp::SDLTexture> texture_image_;
        std::shared_ptr<sdlpp::Font> font_;
        float brightness = 1.0f;
        bool boolen = false;
    };

} // game

#endif //SDL2PP__WINDOW_H_
