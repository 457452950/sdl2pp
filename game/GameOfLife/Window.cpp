#include "Window.h"

namespace gol {

Window::Window() {
    auto render = this->CreateRenderer();

    map_.Resize(20, 20);

    map_.AddCell(12, 12);
    map_.AddCell(12, 13);
    map_.AddCell(12, 14);
    map_.AddCell(13, 13);
    map_.AddCell(11, 13);

    map_.AddCell(15, 12);
    map_.AddCell(15, 13);
    map_.AddCell(15, 14);
}

void Window::RenderProcess(sdlpp::PointF view_pos, double view_angle) {
    auto          render = this->GetRenderer();
    sdlpp::PointF pos{100, 100};

    render->SetDrawColor({255, 255, 255});

    auto &data = map_.GetMap();
    for(int i = 1; i < (data.size() - 1); ++i) {
        for(int j = 1; j < (data[i].size() - 1); ++j) {
            if(data[j][i])
                render->DrawFillRect(SDL_FRect{pos.x + (j * 10), pos.y + (i * 10), 9, 9});
        }
    }

    render->SetDrawColor({0, 0, 0});
    render->SetViewport();
}

void Window::Tick(double_t tick_ms) {
    static uint64_t count = 0;
    count++;
    if((count % 100) == 0)
        map_.Update();
}

} // namespace gol