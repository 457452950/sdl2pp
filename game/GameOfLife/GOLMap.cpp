#include "GOLMap.h"

namespace gol {

void GOLMap::Resize(int width, int height) {
    std::vector<std::vector<bool>> new_map(height + 2, std::vector<bool>(width + 2, false));

    this->map_data_.swap(new_map);
}

std::vector<std::vector<bool>> &GOLMap::Update() {
    std::vector<std::vector<bool>> new_map(this->map_data_.size(), std::vector<bool>(this->map_data_[0].size(), false));

    // width
    for(int w = 1; w < (this->map_data_[0].size() - 1); ++w) {

        // height
        for(int h = 1; h < (this->map_data_.size() - 1); ++h) {
            int live_count = getLiveCount(w, h);

            if(this->map_data_[h][w]) {
                // live
                if(live_count < 2 || live_count > 3) {
                    new_map[h][w] = false;
                } else {
                    new_map[h][w] = true;
                }
            } else {
                // death
                if(live_count == 3) {
                    new_map[h][w] = true;
                }
            }
        }
    }

    this->map_data_.swap(new_map);
    return this->map_data_;
}

int GOLMap::getLiveCount(int w, int h) {
    int count = 0;

    for(int i = -1; i <= 1; ++i) {
        for(int j = -1; j <= 1; ++j) {
            if(i == 0 && j == 0) {
                continue;
            } else {
                if(map_data_[h + i][w + j]) {
                    ++count;
                }
            }
        }
    }

    return count;
}

std::vector<std::vector<bool>> &GOLMap::GetMap() { return map_data_; }

void GOLMap::AddCell(int w, int h) { map_data_[h + 1][w + 1] = true; }

} // namespace gol