#include "Window.h"

#include <filesystem>

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
    sdlpp::PointF pos{0, 0};

    render->SetDrawColor({255, 255, 255});

    auto &data = map_.GetMap();
    for(int h = 1; h < (data.size() - 1); ++h) {
        for(int w = 1; w < (data[h].size() - 1); ++w) {
            if(data[h][w])
                render->DrawFillRect(SDL_FRect{pos.x + (w * 10), pos.y + (h * 10), 9, 9});
        }
    }

    render->SetDrawColor({0, 0, 0});
    render->SetViewport();
}

void Window::Tick(double_t tick_ms) {
    static uint64_t count = 0;
    count++;
    if((count % 30) == 0) {
        map_.Update();
    }
}

bool Window::LoadFile(std::string file) {
    std::filesystem::path file_path{file};
    if(!exists(file_path)) {
        LOG_ERR(log::APP, "file not exist {}", file);
        return false;
    }

    auto ext = file_path.extension().string();
    if(ext == ".cells") {
        return LoadCellsFile(file);
    } else if(ext == ".rle") {
        // todo
    }

    return false;
}

bool Window::LoadCellsFile(std::string file) {
    std::fstream file_(file);

    std::vector<std::tuple<int, int>> cells;

    int w, h = 0;
    while(true) {
        char line_data[2048]{};
        file_.getline(line_data, 2048);

        // jump the commit line
        if(line_data[0] == '!') {
            continue;
        } else if(line_data[0] == '\n' || line_data[0] == '\0') {
            break;
        }

        w = strlen(line_data);
        for(int i = 0; i < w; ++i) {
            if(line_data[i] == '\n') {
                break;
            }
            if(line_data[i] == 'O')
                cells.emplace_back(i, h);
        }

        ++h;


        // file end
        if(file_.eof()) {
            break;
        }
    }

    LOG_INF(log::APP, "w {} h {}", w, h);
    map_.Resize(w + 25, h + 25);
    for(auto &[ww, hh] : cells) {
        map_.AddCell(ww, hh);
    }

    return true;
}

} // namespace gol