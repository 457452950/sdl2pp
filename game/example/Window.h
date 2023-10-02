#pragma once
#ifndef SDL2PP_GAME_WINDOW_H_
#define SDL2PP_GAME_WINDOW_H_

// #include <fmt/format.h>

#include "sdl2pp/SDLpp.h"
#include "sdl2pp/STexture.h"

namespace game {

class Window : public sdlpp::SWindow {
public:
    Window();

    ~Window() override {}

    int KeyEvent(const SDL_KeyboardEvent &event) override {
        LOG_VER(log::APP,
                "scan key = {} {}",
                static_cast<int>(event.keysym.scancode),
                SDL_GetScancodeName(event.keysym.scancode));

        if(event.state != SDL_RELEASED) {
            return SWindow::KeyEvent(event);
        }

        switch(event.keysym.sym) {
        case SDLK_ESCAPE:
        case SDLK_q: {
            this->Close();
            break;
        }
        case SDLK_e: {
            break;
        }
        case SDLK_a: {
            break;
        }
        case SDLK_d: {
            break;
        }
        case SDLK_w: {
            break;
        }
        case SDLK_s: {
            break;
        }
        case SDLK_1: {
            sdlpp::MixChannelManager::GetInstance()->Play(-1, chunk1, 0);
            break;
        }
        case SDLK_2: {
            sdlpp::MixChannelManager::GetInstance()->Play(-1, chunk2, 0);
            break;
        }
        case SDLK_3: {
            sdlpp::MixChannelManager::GetInstance()->Play(-1, chunk3, 0);
            break;
        }
        case SDLK_4: {
            sdlpp::MixChannelManager::GetInstance()->Play(-1, chunk4, 0);
            break;
        }
        case SDLK_9: {
            if(!sdlpp::MixMusicManager::GetInstance()->IsPlaying()) {
                sdlpp::MixMusicManager::GetInstance()->Play(music, 0);
            } else if(sdlpp::MixMusicManager::GetInstance()->Paused()) {
                sdlpp::MixMusicManager::GetInstance()->Resume();
            } else {
                sdlpp::MixMusicManager::GetInstance()->Pause();
            }
            break;
        }
        case SDLK_0: {
            sdlpp::MixMusicManager::GetInstance()->Halt();
            break;
        }
        default:
            LOG_DBG(log::APP, "key = {} {}", event.keysym.sym, SDL_GetKeyName(event.keysym.sym));
            break;
        }
        return SWindow::KeyEvent(event);
    }

    int WindowEvent(const SDL_WindowEvent &event) override {
        switch(event.event) {
        case SDL_WINDOWEVENT_RESIZED: {
            LOG_DBG(log::APP, "resize {} {} ", event.data1, event.data2);

            auto [out_w, out_h] = this->GetRenderer()->GetOutputSize();
            LOG_DBG(log::APP, "output size: {}, {}", out_w, out_h);
            break;
        }
        case SDL_WINDOWEVENT_CLOSE: {
            LOG_DBG(log::APP, "SDL_WINDOW EVENT close but no close");
        }
        }
        return SWindow::WindowEvent(event);
    }

    int MouseButtonEvent(const SDL_MouseButtonEvent &event) override {
        LOG_DBG(log::APP,
                "mouse button state {} x {} y {} button {} click {}",
                event.state,
                event.x,
                event.y,
                event.button,
                event.clicks);
        return SWindow::MouseButtonEvent(event);
    }

    int MouseMoveEvent(const SDL_MouseMotionEvent &event) override {
        LOG_VER(log::APP,
                "mouse motion state {} x {} y {}  xrel {} yrel {}",
                event.state,
                event.x,
                event.y,
                event.xrel,
                event.yrel);
        return SWindow::MouseMoveEvent(event);
    }

    int MouseWheelEvent(const SDL_MouseWheelEvent &event) override {
        LOG_DBG(log::APP,
                "mouse wheel state {} x {} y {} direction {}",
                event.windowID,
                event.x,
                event.y,
                event.direction);
        return SWindow::MouseWheelEvent(event);
    }

    void RenderProcess(sdlpp::PointF view_pos, double view_angle) override;

    void Tick(double tick_ms) override;


private:
    std::shared_ptr<sdlpp::STexture> texture_man_;
    std::shared_ptr<sdlpp::STexture> texture_image_;
    std::shared_ptr<sdlpp::STexture> texture_image2_;

    uint16_t mix = 255;

    std::vector<sdlpp::SDLTexture::BlendMode> values = {
            sdlpp::SDLTexture::BlendMode::SDL_BLENDMODE_NONE,
            sdlpp::SDLTexture::BlendMode::SDL_BLENDMODE_BLEND,
            sdlpp::SDLTexture::BlendMode::SDL_BLENDMODE_ADD,
            sdlpp::SDLTexture::BlendMode::SDL_BLENDMODE_MOD,
            sdlpp::SDLTexture::BlendMode::SDL_BLENDMODE_MUL,
    };
    int index = 1;

    double angle_{0};

    std::shared_ptr<sdlpp::Font> font_;

    std::shared_ptr<sdlpp::MixMusic> music;

    std::shared_ptr<sdlpp::MixChunk> chunk1;
    std::shared_ptr<sdlpp::MixChunk> chunk2;
    std::shared_ptr<sdlpp::MixChunk> chunk3;
    std::shared_ptr<sdlpp::MixChunk> chunk4;

    sdlpp::SAnimation animation;
};

} // namespace game

#endif // SDL2PP_GAME_WINDOW_H_
