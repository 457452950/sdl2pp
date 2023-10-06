#pragma once
#ifndef SDL2PP_GAME_GAMEOFLIFE_GOLMAP_H_
#define SDL2PP_GAME_GAMEOFLIFE_GOLMAP_H_

#include <cstdint>
#include <vector>
#include <bitset>
#include <unordered_map>

namespace gol {

class GOLMap {
public:
    GOLMap()  = default;
    ~GOLMap() = default;

    void Resize(int width, int height);

    void AddCell(int w, int h);

    void Clear() { map_data_.clear(); }

    std::vector<std::vector<bool>> &Update();

    std::vector<std::vector<bool>> &GetMap();

private:
    int getLiveCount(int w, int h);

private:
    std::vector<std::vector<bool>> map_data_;
};

} // namespace gol

#endif // SDL2PP_GAME_GAMEOFLIFE_GOLMAP_H_
