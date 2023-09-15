#include "sdl2pp/SDLpp.h"

#include "Window.h"

int main(int argc, char *argv[]) {
    auto ok = sdlpp::SDL::Init(sdlpp::SDL::EVERY_THING);
    if (!ok) {
        SDL_LogError(SDL_LOG_CATEGORY_APPLICATION, "Failed to init SDL %s", SDL_GetError());
        return 1;
    }
    ok = sdlpp::SDL::GetInstance()->InitImg(sdlpp::SDL::PNG | sdlpp::SDL::JPG);
    if (!ok) {
        SDL_LogError(SDL_LOG_CATEGORY_APPLICATION, "Failed to init SDL_image %s", SDL_GetError());
        return 1;
    }
    ok = sdlpp::SDL::GetInstance()->InitTTF();
    if (!ok) {
        SDL_LogError(SDL_LOG_CATEGORY_APPLICATION, "Failed to init SDL_ttf %s", SDL_GetError());
        return 1;
    }

    game::Window window;

    SDL_Log("dispaly count : %d", sdlpp::Display::GetInstance()->GetDisplayCount());

    auto &display_infos = sdlpp::Display::GetInstance()->GetDisplayInfos();
    for (auto &info: display_infos) {
        SDL_Log("display index %d name : %s", info.GetIndex(), info.GetName().data());

        auto &mode = info.GetMode();
        SDL_Log("display mode : %d, %d, %d, %s", mode.w, mode.h,
                mode.refresh_rate, SDL_GetPixelFormatName(mode.format));

        SDL_Log("display bounds : %d, %d, %d, %d", info.GetTotalBounds().x, info.GetTotalBounds().y,
                info.GetTotalBounds().w, info.GetTotalBounds().h);
        SDL_Log("display usable bounds : %d, %d, %d, %d", info.GetUsableBounds().x,
                info.GetUsableBounds().y, info.GetUsableBounds().w, info.GetUsableBounds().h);

        auto &supported_modes = info.GetSupportedModes();
        for (auto &mode: supported_modes) {
            SDL_Log("supported mode : %d, %d, %d, %s", mode.w, mode.h,
                    mode.refresh_rate, SDL_GetPixelFormatName(mode.format));
        }
    }

    auto renderer_count = SDL_GetNumRenderDrivers();
    SDL_Log("%d render drivers", renderer_count);
    for (int i = 0; i < renderer_count; ++i) {
        SDL_RendererInfo info;
        SDL_GetRenderDriverInfo(i, &info);
        SDL_Log("Renderer %d: %s", i, info.name);
        if (info.flags & SDL_RENDERER_SOFTWARE) {
            SDL_Log("Renderer %d is software", i);
        }
        if (info.flags & SDL_RENDERER_ACCELERATED) {
            SDL_Log("Renderer %d is accelerated", i);
        }
        if (info.flags & SDL_RENDERER_PRESENTVSYNC) {
            SDL_Log("Renderer %d supports vsync", i);
        }
        if (info.flags & SDL_RENDERER_TARGETTEXTURE) {
            SDL_Log("Renderer %d supports render to texture", i);
        }
        SDL_Log("Renderer available %d texture formats.", info.num_texture_formats);
        for (int ii = 0; ii < info.num_texture_formats; ++ii) {
            SDL_Log("Renderer %d texture format %d: %s", i, ii, SDL_GetPixelFormatName(info.texture_formats[ii]));
        }
        SDL_Log("Renderer %d max texture width: %d", i, info.max_texture_width);
        SDL_Log("Renderer %d max texture height: %d", i, info.max_texture_height);
    }


    return window.Exec();
}
