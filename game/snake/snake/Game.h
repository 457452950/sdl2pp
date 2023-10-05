#pragma once
#ifndef SDL2PP_GAME_SNAKE_SNAKE_GAME_H_
#define SDL2PP_GAME_SNAKE_SNAKE_GAME_H_

#include <memory>

#include "sdl2pp/SDLpp.h"

namespace snake {

class Game {
public:
    // 单例
    static Game *Instance() {
        static Game game;
        return &game;
    }

    ~Game() = default;

    bool Init(sdlpp::SWindow *window);

    void Render(std::shared_ptr<sdlpp::SRenderer> renderer);

    void Reset(int width, int height);

    void Close();

    enum Input { UP, DOWN, LEFT, RIGHT };
    void Step(Input input);


private:
    Game() = default;
};

} // namespace snake

#endif // SDL2PP_GAME_SNAKE_SNAKE_GAME_H_
