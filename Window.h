#pragma once
#ifndef SDL2PP__WINDOW_H_
#define SDL2PP__WINDOW_H_

#include <fmt/format.h>

#include "sdl2pp/SWindow.h"

namespace game {

    class Window : public sdlpp::SWindow {
    public:
        Window() {
            SDL_RendererInfo info;
            SDL_GetRendererInfo(this->GetRenderer(), &info);
            SDL_Log("Renderer name: %s", info.name);
            if (info.flags & SDL_RENDERER_SOFTWARE) {
                SDL_Log("Renderer is software");
            }
            if (info.flags & SDL_RENDERER_ACCELERATED) {
                SDL_Log("Renderer is accelerated");
            }
            if (info.flags & SDL_RENDERER_PRESENTVSYNC) {
                SDL_Log("Renderer supports vsync");
            }
            if (info.flags & SDL_RENDERER_TARGETTEXTURE) {
                SDL_Log("Renderer supports render to texture");
            }
            SDL_Log("Renderer available %d texture formats.", info.num_texture_formats);
            for (int ii = 0; ii < info.num_texture_formats; ++ii) {
                SDL_Log("Renderer texture format %d: %s", ii, SDL_GetPixelFormatName(info.texture_formats[ii]));
            }
            SDL_Log("Renderer max texture width: %d", info.max_texture_width);
            SDL_Log("Renderer max texture height: %d", info.max_texture_height);

            image_ = sdlpp::loadTexture("H:/Resources/cos.jpg", this->GetRenderer());
            if (image_ == nullptr) {
                SDL_LogError(SDL_LOG_CATEGORY_APPLICATION, "One or more textures failed to load. %s\n", SDL_GetError());
                SDL_assert(image_);
            }
        }

        ~Window() override {
            SDL_DestroyTexture(image_);
        }

        int KeyEvent(const SDL_KeyboardEvent &event) override {
            SDL_Log("scan key = %d %s", event.keysym.scancode,
                    SDL_GetScancodeName(event.keysym.scancode));
            switch (event.keysym.sym) {
                case SDLK_ESCAPE:
                case SDLK_q: {
                    this->Close();
                    break;
                }
                case SDLK_e : {
                    boolen = !boolen;
                    SDL_Log("boolen = %d", boolen);
                    this->SetBordered(boolen);
                    break;
                }
                case SDLK_a: {
                    brightness -= 0.1f;
                    SDL_Log("brightness = %f", brightness);
                    this->SetBrightness(brightness);
                    break;
                }
                case SDLK_d: {
                    brightness += 0.1f;
                    SDL_Log("brightness = %f", brightness);
                    this->SetBrightness(brightness);
                    break;
                }
                case SDLK_w: {
                    this->SetFullscreen(true);
                    break;
                }
                case SDLK_s: {
                    this->SetFullscreen(false);
                    break;
                }
                default:
                    SDL_Log("key = %d %s", event.keysym.sym, SDL_GetKeyName(event.keysym.sym));
                    break;
            }
            return SWindow::KeyEvent(event);
        }

        int WindowEvent(const SDL_WindowEvent &event) override {
            switch (event.event) {
                case SDL_WINDOWEVENT_RESIZED: {
                    SDL_Log("resize %d %d ", event.data1, event.data2);
                    break;
                }
                case SDL_WINDOWEVENT_CLOSE: {
                    SDL_LogDebug(0, "SDL_WINDOW EVENT close but no close");
                }
            }
            return SWindow::WindowEvent(event);
        }

        void RenderProcess() override {
            int m_x, m_y;
            auto s = SDL_GetMouseState(&m_x, &m_y);
            std::string text = fmt::format("{}, {} : {}", s, m_x, m_y);

//        SDL_Log("%s", text.c_str());

            SDL_Color color = {255, 255, 255, 255};
            SDL_Texture *txt_image = sdlpp::renderText(text, R"(H:\Resources\ttf\hk4e_zh-cn.ttf)",
                                                       color, 64, this->GetRenderer());
            if (txt_image == nullptr) {
                SDL_Log("render text error %s", SDL_GetError());
                SDL_assert(txt_image);
                return;
            }
            //Get the texture w/h so we can center it in the screen
            int iW, iH;
            SDL_QueryTexture(txt_image, nullptr, nullptr, &iW, &iH);
            int x = this->GetWidth() / 2 - iW / 2;
            int y = this->GetHeight() / 2 - iH / 2;

            sdlpp::renderTexture(image_, this->GetRenderer(), 0, 0);
            sdlpp::renderTexture(txt_image, this->GetRenderer(), x, y);

            SDL_DestroyTexture(txt_image);
        }

    private:
        SDL_Texture *image_;
        float brightness = 1.0f;
        bool boolen = false;
    };

} // game

#endif //SDL2PP__WINDOW_H_
