#include "Window.h"

#include "tools/Resource.h"

namespace game {

Window::Window() {
    this->SetTitle("Snake");

    // init ttf font
    font_ = sdlpp::Font::Open(R"(H:\Resources\ttf\hk4e_zh-cn.ttf)", 64);
    if(font_ == nullptr) {
        LOG_ERR(log::APP, "Could not load font: {}", SDL_GetError());
        return;
    }

    // set icon
    auto icon = Resource::LoadFile("snake.jpg");
    this->SetIcon(icon);

    this->SetPhysicPerS(0);
}

void Window::RenderProcess(sdlpp::PointF view_pos, double view_angle) {}

} // namespace game