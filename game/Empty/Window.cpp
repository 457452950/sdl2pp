#include "Window.h"

namespace game {

Window::Window() { auto render = this->CreateRenderer(); }

void Window::RenderProcess(sdlpp::PointF view_pos, double view_angle) {}

void Window::Tick(double_t tick_ms) {}

} // namespace game