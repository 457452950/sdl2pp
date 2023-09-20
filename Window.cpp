#include "Window.h"

#include "sdl2pp/SDLpp.h"

namespace game {
    Window::Window() {

        auto render = this->CreateRenderer();

        SDL_RendererInfo info;
        render->GetRendererInfo(&info);
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

        image_ = sdlpp::IMG_LoadTextureFromFile("H:/Resources/cos.jpg", this->GetRenderer());
        if (image_ == nullptr) {
            SDL_LogError(SDL_LOG_CATEGORY_APPLICATION, "One or more textures failed to load. %s\n", SDL_GetError());
            SDL_assert(image_);
        }
    }

    void Window::RenderProcess() {
        int m_x, m_y;
        auto s = SDL_GetMouseState(&m_x, &m_y);
        std::string text = fmt::format("{}, {} : {}", s, m_x, m_y);

        SDL_Color color = {255, 255, 255, 200};
        SDL_Texture *txt_image = sdlpp::renderText(text, R"(H:\Resources\ttf\hk4e_zh-cn.ttf)",
                                                   color, 64, this->GetRenderer()->Get());
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

        sdlpp::renderTexture(image_->Get(), this->GetRenderer()->Get(), 0, 0);
        sdlpp::renderTexture(txt_image, this->GetRenderer()->Get(), x, y);

        SDL_DestroyTexture(txt_image);
    }
} // game