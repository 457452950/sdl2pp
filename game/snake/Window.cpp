#include "Window.h"

#include "tools/Resource.h"
#include "ui/UI.h"

Window::Window() {
    this->SetTitle("Snake");

    // set icon
    auto icon = snake::Resource::GetInstance()->GetImage("snake.jpg");
    this->SetIcon(icon);

    this->SetPhysicPerS(0);

    auto render = this->CreateRenderer();

    ui::UI::GetInstance()->Init(this);

    this->SetResizeable(true);
}

void Window::RenderProcess(sdlpp::PointF view_pos, double view_angle) {
    ui::UI::GetInstance()->Render(this->GetRenderer());
}

Window::~Window() { ui::UI::GetInstance()->Clear(); }

int Window::WindowEvent(const SDL_WindowEvent &event) {
    switch(event.event) {
    case SDL_WINDOWEVENT_RESIZED: {
        LOG_DBG(log::APP, "resize {} {} ", event.data1, event.data2);

        auto [out_w, out_h] = this->GetRenderer()->GetOutputSize();
        LOG_DBG(log::APP, "output size: {}, {}", out_w, out_h);

        ui::UI::GetInstance()->Resize(event.data1, event.data2);
        break;
    }
    case SDL_WINDOWEVENT_CLOSE: {
        LOG_DBG(log::APP, "SDL_WINDOW EVENT close but no close");
    }
    }
    return SWindow::WindowEvent(event);
}
