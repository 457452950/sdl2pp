#pragma once
#ifndef SDL2PP_GAME_SNAKE_SNAKE_GAME_H_
#define SDL2PP_GAME_SNAKE_SNAKE_GAME_H_

#include <memory>
#include <map>

#include "sdl2pp/SDLpp.h"

namespace snake {

class SnakeGame {
    static constexpr int MAPMINW = 5;
    static constexpr int MAPMAXW = 50;
    static constexpr int MAPMINH = 5;
    static constexpr int MAPMAXH = 50;

public:
    //
    void                 Reset(int width, int height);
    //
    std::tuple<int, int> GetMapSize() { return {width_, height_}; }

    enum MapItem : int {
        NORMAL = 0,
        WALL, // 墙体
        SNAKE_HEAD,
        SNAKE_BODY,
        FRUIT, // 水果
        EGDE,  // 地图边缘
    };

    enum Input { UP, DOWN, LEFT, RIGHT };
    void Step(Input input);

    struct StartConf {
        sdlpp::PointI begin;
        Input         input{UP};
    };
    void Start(const StartConf &conf);
    void Close() {}

protected:
    // override
    virtual void onResizeMap(int w, int h) {}
    enum Next { DIE = 0, CONTINUE };
    virtual Next onArrived(const sdlpp::PointI &pos, int item);

protected:
    using MapDataType = std::vector<int>;
    using MapChangeList =
            std::map<sdlpp::PointI, int, std::function<bool(const sdlpp::PointI &, const sdlpp::PointI &)>>;

    static bool _compare_point(const sdlpp::PointI &left, const sdlpp::PointI &right) {
        return left.x < right.x && left.y < right.y;
    }

    void               SetMapItem(sdlpp::PointI pos, int item);
    const MapDataType &GetMapData() { return this->map_data_; }
    MapChangeList      GetMapChangeList();

    int getIndex(int w, int h) const { return w + width_ * h; }
    int getIndex(sdlpp::PointI pos) const { return pos.x + width_ * pos.y; }

private:
    int                      width_{0}, height_{0};
    MapDataType              map_data_;
    MapChangeList            change_list_{SnakeGame::_compare_point};
    std::list<sdlpp::PointI> snake_;
};

class Game : public SnakeGame {
public:
    // 单例
    static Game *Instance() {
        static Game game;
        return &game;
    }

    ~Game() = default;

    bool Init(sdlpp::SWindow *window);

    void Render(std::shared_ptr<sdlpp::SRenderer> renderer);

    // override
    void onResizeMap(int w, int h) override;
    Next onArrived(const sdlpp::PointI &pos, int item) override;

private:
    Game() = default;
};


} // namespace snake

#endif // SDL2PP_GAME_SNAKE_SNAKE_GAME_H_
