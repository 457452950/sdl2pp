#include "SDLTTF.h"

namespace sdlpp {
    std::shared_ptr<Font> Font::Open(std::string_view path, int point_size) {
        return Font::Create(TTF_OpenFont(path.data(), point_size));
    }

    std::shared_ptr<Font> Font::Open(std::string_view path, int point_size, long index) {
        return Font::Create(TTF_OpenFontIndex(path.data(), point_size, index));
    }

    std::shared_ptr<Font> Font::Open(std::string_view path, int point_size, unsigned int hdpi, unsigned int vdpi) {
        return Font::Create(TTF_OpenFontDPI(path.data(), point_size, hdpi, vdpi));
    }

    std::shared_ptr<Font>
    Font::Open(std::string_view path, int point_size, long index, unsigned int hdpi, unsigned int vdpi) {
        return Font::Create(TTF_OpenFontIndexDPI(path.data(), point_size, index, hdpi, vdpi));
    }
} // sdlpp