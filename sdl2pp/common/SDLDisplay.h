#pragma once
#ifndef SDL2_BASE_DISPLAY_H
#define SDL2_BASE_DISPLAY_H

#include <vector>
#include <string>

#include <SDL_video.h>

#include "Rect.h"

namespace sdlpp {

class DisplayInfo {
public:
    explicit DisplayInfo(int index) : index_(index) {
        name_ = SDL_GetDisplayName(index);

        SDL_GetDesktopDisplayMode(index, &mode_);

        SDL_GetDisplayBounds(index, &total_bounds_);
        SDL_GetDisplayUsableBounds(index, &usable_bounds_);


        auto count = SDL_GetNumDisplayModes(index_);
        supported_modes_.resize(count);
        for(auto i = 0; i < count; i++) {
            SDL_GetDisplayMode(index_, i, &supported_modes_[i]);
        }
    }

    ~DisplayInfo() = default;

    int GetIndex() const { return index_; }

    std::string_view GetName() const { return name_; }

    const SDL_DisplayMode &GetMode() const { return mode_; }

    const std::vector<SDL_DisplayMode> &GetSupportedModes() const { return supported_modes_; }

    const RectI &GetTotalBounds() const { return total_bounds_; }

    const RectI &GetUsableBounds() const { return usable_bounds_; }

private:
    int                          index_{};
    std::string                  name_{};
    SDL_DisplayMode              mode_{};
    RectI                        total_bounds_{};
    RectI                        usable_bounds_{};
    std::vector<SDL_DisplayMode> supported_modes_{};
};

// displays config
class Display {
private:
    Display();

public:
    ~Display() = default;

    static Display *GetInstance() {
        static auto *instance_ = new Display;
        return instance_;
    }

    size_t GetDisplayCount() const { return display_infos_.size(); }

    const std::vector<DisplayInfo> &GetDisplayInfos() const { return display_infos_; }

private:
    std::vector<DisplayInfo> display_infos_;
};

} // namespace sdlpp

#endif // SDL2_BASE_DISPLAY_H
