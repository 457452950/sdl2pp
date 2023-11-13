#include "Window.h"

namespace game {

Window::Window() {
    auto render = this->CreateRenderer();

    render->SetDrawColor({255, 255, 255, 255});
}

void Window::RenderProcess() {}

void Window::Tick(double_t tick_ms) {}

} // namespace game