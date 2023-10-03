#include "Resource.h"

#include <filesystem>

namespace game {

std::shared_ptr<sdlpp::SDLSurface> Resource::LoadFile(const std::string &file_name) {
    auto _file_name = base_path + file_name;
    if(!std::filesystem::exists(_file_name)) {
        LOG_ERR(log::APP, "load file failed, file not found: {} ", _file_name);
        return nullptr;
    }

    auto surface = sdlpp::IMG_LoadSurfaceFromFile(_file_name);
    if(!surface) {
        LOG_ERR(log::APP, "load from file {} failed: {} ", _file_name, SDL_GetError());
    }

    return surface;
}


} // namespace game