#pragma once
#ifndef SDL2PP_SANIMATION_H_
#define SDL2PP_SANIMATION_H_

#include "common/Point.h"

#include "STexture.h"
#include "Renderable.h"


namespace sdlpp {

class SAnimation : public Renderable {
public:
    SAnimation()  = default;
    ~SAnimation() = default;

    void AddFrame(std::shared_ptr<SDLTexture> texture) { this->textures_vec_.emplace_back(texture, nullptr); }
    void AddFrame(std::shared_ptr<SDLTexture> texture, const RectI &rect) {
        this->textures_vec_.emplace_back(texture, &rect);
    }
    void AddFrame(const STexture &texture) { this->textures_vec_.push_back(texture); }

    size_t   GetFrameCount() { return textures_vec_.size(); }
    STexture GetFrame(int index) { return textures_vec_[index]; }

    void Update(int offset = 1) {
        index_ += offset;
        index_ %= (int)textures_vec_.size();
    }

    void SetIndex(int index) { index_ = index % (int)textures_vec_.size(); }
    int  GetIndex() const { return index_; }
    void Reset() { index_ = 0; }

    void Clear() { this->textures_vec_.clear(); }

    void Render(std::shared_ptr<SRenderer> renderer, PointF base_pos, double base_angle, sdlpp::Scale scale) override {
        textures_vec_[index_].Render(renderer, base_pos + this->position_, base_angle + this->angle_, scale * scale_);
    }

private:
    int index_{0};

    std::vector<STexture> textures_vec_;
};

} // namespace sdlpp

#endif // SDL2PP_SANIMATION_H_
