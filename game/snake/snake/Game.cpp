#include "Game.h"

namespace snake {

int Check(int _max, int _min, int value) { return std::min<int>(std::max<int>(_min, value), _max); }

void SnakeGame::Reset(int width, int height) {
    width_  = Check(MAPMAXW, MAPMINW, width);
    height_ = Check(MAPMAXH, MAPMINH, height);

    this->map_data_.resize(width_ * height_, NORMAL);

    this->onResizeMap(width_, height_);
}

void SnakeGame::Start(const StartConf &conf) {
    // todo:
}

void SnakeGame::Step(SnakeGame::Input input) {
    // todo:
}

void SnakeGame::SetMapItem(sdlpp::PointI pos, int item) {
    auto index = getIndex(pos.x, pos.y);

    if(this->map_data_[index] != item) {
        this->map_data_[index] = item;
        this->change_list_.insert({pos, item});
    }
}

SnakeGame::MapChangeList SnakeGame::GetMapChangeList() {
    MapChangeList t;
    t.swap(this->change_list_);
    return t;
}

SnakeGame::Next SnakeGame::onArrived(const sdlpp::PointI &pos, int item) {
    switch(item) {
    case WALL:
    case SNAKE_HEAD:
    case SNAKE_BODY:
        return DIE;
    default:
        return CONTINUE;
    }
}

bool Game::Init(sdlpp::SWindow *window) {
    // todo:
    return true;
}

void Game::Render(std::shared_ptr<sdlpp::SRenderer> renderer) {
    auto changeList = this->GetMapChangeList();
    if(changeList.empty()) {
        return;
    }

    auto [w, h] = this->GetMapSize();
    auto map    = this->GetMapData();
    for(int i = 0; i < h; ++i) {
        for(int j = 0; j < w; ++j) {
            fmt::print("{} ", map[getIndex(j, i)]);
        }
        fmt::print("\n");
    }
    fmt::print("\n");
}

void Game::onResizeMap(int w, int h) {
    // init wall
    for(int i = 0; i < w; ++i) {
        this->SetMapItem({i, h - 1}, WALL);
        this->SetMapItem({i, 0}, WALL);
    }
    for(int i = 0; i < h; ++i) {
        this->SetMapItem({w - 1, i}, WALL);
        this->SetMapItem({0, i}, WALL);
    }
}

SnakeGame::Next Game::onArrived(const sdlpp::PointI &pos, int item) {
    // todo:
    return SnakeGame::onArrived(pos, item);
}

} // namespace snake