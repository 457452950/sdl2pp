#pragma once
#ifndef SDL2PP_GAME_WINDOW_H_
#define SDL2PP_GAME_WINDOW_H_

#include "sdl2pp/GLWindow.hpp"

#include "Shader.h"
#include "FPSCamera.hpp"
#include "gl/GLObject.hpp"

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
                canMove = true;
            } else {
                canMove = false;
            }
        } break;
        }
        return SWindow::MouseButtonEvent(event);
    }

    int MouseMoveEvent(const SDL_MouseMotionEvent &event) override {
        if(canMove)
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
    std::shared_ptr<Shader> light_shader_;
    std::shared_ptr<Shader> light_source_shader_;

    std::shared_ptr<FPSCamera> camera_;
    bool                       canMove = false;

    VAO light_vao, cube_vao;
    VBO cube_vbo;
    EBO cube_ebo;

    float     r{1};
    float     speed{1.0f};
    float     theta{0.0f};
    glm::vec3 lightPos{1.0f, 0.0f, 0.0f};

    float ambientLight{0.7f};
    float diffuseLight{0.2f};
    float specularLight{1.0f};

    bool ambient_enable_{true};
    bool diffuse_enable_{true};
    bool specular_enable_{true};

    int speed_x{0};
    int speed_y{0};
};

} // namespace game

#endif // SDL2PP_GAME_WINDOW_H_
