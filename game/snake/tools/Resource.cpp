#include "Resource.h"

#include <filesystem>

namespace snake {

std::shared_ptr<sdlpp::SDLSurface> Resource::GetImage(const std::string &file_name) {
    // try to find
    auto it = this->images_map_.find(file_name);
    if(it != this->images_map_.end()) {
        return it->second;
    }

    // get file path
    auto _file_name = base_path + image_path + file_name;
    if(!std::filesystem::exists(_file_name)) {
        LOG_ERR(log::APP, "load file failed, file not found: {} ", _file_name);
        return nullptr;
    }

    // load file
    auto surface = sdlpp::IMG_LoadSurfaceFromFile(_file_name);
    if(!surface) {
        LOG_ERR(log::APP, "load from file {} failed: {} ", _file_name, SDL_GetError());
        return nullptr;
    }

    this->images_map_[file_name] = surface;

    return surface;
}

std::shared_ptr<sdlpp::Font> Resource::GetFont(const std::string &file_name, int point_size) {
    // try to find
    auto it = this->fonts_map_.find(file_name);
    if(it != this->fonts_map_.end()) {
        auto font = it->second;
        if(font->GetFontSize() != point_size) {
            auto ok = font->SetFontSize(point_size);
            if(!ok) {
                LOG_ERR(log::APP, "resize font point size fail, err: {}.", SDL_GetError());
                return nullptr;
            }
        }
        return font;
    }

    // get file path
    auto _file_name = base_path + font_path + file_name;
    if(!std::filesystem::exists(_file_name)) {
        LOG_ERR(log::APP, "load file failed, file not found: {} ", _file_name);
        return nullptr;
    }

    // load file
    auto font = sdlpp::Font::Open(_file_name, point_size);
    if(!font) {
        LOG_ERR(log::APP, "load from file {} failed: {} ", _file_name, SDL_GetError());
        return nullptr;
    }

    this->fonts_map_[file_name] = font;

    return font;
}


} // namespace snake