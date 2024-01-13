#include "Window.h"

#include "tools/Resource.h"
#include "ui/UI.h"
#include "snake/Game.h"

Window::Window() {
    this->SetTitle("Snake");

    // set icon
    auto icon = snake::Resource::GetInstance()->GetImage("snake.jpg");
    this->SetIcon(icon);

    this->SetPhysicPerS(0);

    auto render = this->CreateRenderer();

    this->SetResizeable(true);
    this->SetFps(120);

    // init sub
    ui::UI::GetInstance()->Init(this);
    auto game = snake::Game::Instance();
    game->Init(this);
    game->Reset(10, 15);
    game->Start(snake::SnakeGame::StartConf{{3, 3}});
}

void Window::RenderProcess() {
    ui::UI::GetInstance()->Render(this->GetRenderer());
    snake::Game::Instance()->Render(this->GetRenderer());
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

void Window::RenderFlush() { this->GetRenderer()->Flush(); }
