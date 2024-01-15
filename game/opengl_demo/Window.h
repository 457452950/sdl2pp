#pragma once
#ifndef SDL2PP_GAME_WINDOW_H_
#define SDL2PP_GAME_WINDOW_H_

#include "sdl2pp/GLWindow.hpp"

#include "Shader.h"
#include "FPSCamera.hpp"

namespace game {

class Window : public sdlpp::GlWindow {
public:
    Window();

    ~Window() override;

    int KeyEvent(const SDL_KeyboardEvent &event) override {
        LOG_INF(log::APP, "keyboard event");

        switch(event.keysym.scancode) {
        case SDL_SCANCODE_UNKNOWN:
            break;
        case SDL_SCANCODE_A:
            if(event.state == SDL_PRESSED && speed_y != 1) {
                speed_y += 1;
            } else if(event.state == SDL_RELEASED) {
                speed_y -= 1;
            }
            break;
        case SDL_SCANCODE_D:
            if(event.state == SDL_PRESSED && speed_y != -1) {
                speed_y -= 1;
            } else if(event.state == SDL_RELEASED) {
                speed_y += 1;
            }
            break;
        case SDL_SCANCODE_S:
            if(event.state == SDL_PRESSED && speed_x != -1) {
                speed_x -= 1;
            } else if(event.state == SDL_RELEASED) {
                speed_x += 1;
            }
            break;
        case SDL_SCANCODE_W:
            if(event.state == SDL_PRESSED && speed_x != 1) {
                speed_x += 1;
            } else if(event.state == SDL_RELEASED) {
                speed_x -= 1;
            }
            break;
        case SDL_SCANCODE_ESCAPE:
            this->Close();
            break;
        default:
            break;
        }
        LOG_INF(log::APP, "{} {}", speed_x, speed_y);
        return SWindow::KeyEvent(event);
    }

    int WindowEvent(const SDL_WindowEvent &event) override { return SWindow::WindowEvent(event); }

    int MouseButtonEvent(const SDL_MouseButtonEvent &event) override { return SWindow::MouseButtonEvent(event); }

    int MouseMoveEvent(const SDL_MouseMotionEvent &event) override {
        camera_->ProcessMouseMovement(event.xrel, -event.yrel);
        return SWindow::MouseMoveEvent(event);
    }

    int MouseWheelEvent(const SDL_MouseWheelEvent &event) override {
        camera_->ProcessMouseScroll(event.preciseY);
        return SWindow::MouseWheelEvent(event);
    }

    void RenderClear() override;
    void RenderProcess() override;
    void RenderFlush() override;

    void Tick(double tick_ms) override;

private:
    std::shared_ptr<Shader>    shader_;
    std::shared_ptr<FPSCamera> camera_;

    int speed_x{0};
    int speed_y{0};
};

} // namespace game

#endif // SDL2PP_GAME_WINDOW_H_
