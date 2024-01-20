#pragma once
#ifndef SDL2PP_GAME_WINDOW_H_
#define SDL2PP_GAME_WINDOW_H_

#include "sdl2pp/GLWindow.hpp"

#include "FPSCamera.hpp"
#include "gl/GLObject.hpp"
#include "gl/Shader.hpp"
#include "gl/Mesh.hpp"
#include "game/opengl_demo/gl/Model.hpp"

namespace game {

class Window : public sdlpp::GlWindow {
public:
    Window();

    ~Window() override;

    void eventHandle(const SDL_Event &event) override;

    int KeyEvent(const SDL_KeyboardEvent &event) override {
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
        return SWindow::KeyEvent(event);
    }

    int WindowEvent(const SDL_WindowEvent &event) override { return GlWindow::WindowEvent(event); }

    int MouseButtonEvent(const SDL_MouseButtonEvent &event) override {
        switch(event.button) {
        case SDL_BUTTON_RIGHT: {
            if(event.type == SDL_MOUSEBUTTONDOWN) {
                can_move_ = true;
            } else {
                can_move_ = false;
            }
        } break;
        }
        return SWindow::MouseButtonEvent(event);
    }

    int MouseMoveEvent(const SDL_MouseMotionEvent &event) override {
        if(can_move_)
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

protected:
    void IMGUIProcess();

private:
    std::shared_ptr<gl::Shader> model_shader_;

    std::shared_ptr<FPSCamera> camera_;
    bool                       can_move_ = false;

    int speed_x{0};
    int speed_y{0};

    std::shared_ptr<gl::Model> model_;
    std::shared_ptr<gl::Mesh>  mesh_;
};

} // namespace game

#endif // SDL2PP_GAME_WINDOW_H_
