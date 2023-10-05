#include "UI.h"

#include "../tools/Resource.h"

namespace ui {

bool UI::Init(sdlpp::SWindow *window) {
    auto renderer = window->GetRenderer();

    if(!this->InitTitle(renderer)) {
        return false;
    }
    if(!this->InitHelpInfo(renderer)) {
        return false;
    }


    auto [w_w, w_h] = window->GetSize();
    this->Resize(w_w, w_h);

    return true;
}

void UI::Clear() {
    this->title_.reset();
    this->help_info_.clear();
}

void UI::Render(std::shared_ptr<sdlpp::SRenderer> renderer) {
    renderer->SetDrawColor({255, 255, 255});
    //    renderer->SetViewport(render_view_);

    this->render(renderer);

    renderer->SetViewport();
    renderer->SetDrawColor({0, 0, 0});
}

void UI::render(std::shared_ptr<sdlpp::SRenderer> renderer) {
    // draw outline rect
    renderer->DrawRect(SDL_Rect{distance_, distance_, render_view_.w - 2 * distance_, render_view_.h - 2 * distance_});

    // render title
    this->title_->Render(renderer);

    //     render help info
    for(auto &it : help_info_) {
        it->Render(renderer);
    }
    //    help_info_[0]->Render(renderer);
}

bool UI::InitTitle(std::shared_ptr<sdlpp::SRenderer> renderer) {
    auto font = snake::Resource::GetInstance()->GetFont("hk4e_zh-cn.ttf", 34);
    if(font == nullptr) {
        LOG_ERR(log::APP, "Failed to load font : {}", SDL_GetError());
        return false;
    }

    auto surface = font->RenderText_Solid(TITLE, {255, 255, 255});
    if(surface == nullptr) {
        LOG_ERR(log::APP, "Failed to render text snake err: {}", SDL_GetError());
        return false;
    }

    auto texture = renderer->CreateTextureFromSurface(surface);
    if(texture == nullptr) {
        LOG_ERR(log::APP, "Failed to create texture from surface : {}", SDL_GetError());
        return false;
    }

    this->title_ = std::make_shared<sdlpp::STexture>(texture);

    return true;
}

bool UI::InitHelpInfo(std::shared_ptr<sdlpp::SRenderer> renderer) {
    auto font = snake::Resource::GetInstance()->GetFont("hk4e_zh-cn.ttf", 17);
    if(font == nullptr) {
        LOG_ERR(log::APP, "Failed to load font : {}", SDL_GetError());
        return false;
    }

    for(auto i : HELP_INFO) {
        auto surface = font->RenderText_Solid(i, {255, 255, 255});
        if(surface == nullptr) {
            LOG_ERR(log::APP, "Failed to render text snake err: {}", SDL_GetError());
            return false;
        }

        auto texture = renderer->CreateTextureFromSurface(surface);
        if(texture == nullptr) {
            LOG_ERR(log::APP, "Failed to create texture from surface : {}", SDL_GetError());
            return false;
        }

        this->help_info_.push_back(std::make_shared<sdlpp::STexture>(texture));
    }

    return true;
}

void UI::Resize(int new_w, int new_h) {
    this->render_view_ = {new_w - 200, 0, 200, new_h};

    // move title to w_center
    auto w_center  = 200 / 2;
    w_center      -= (this->title_->GetTexture()->GetWidth() / 2);
    LOG_DBG(log::APP, "w_center : {}", w_center);
    this->title_->MoveTo({(float)w_center, 40.0});


    for(int i = 0; i < this->help_info_.size(); ++i) {
        w_center = (200 / 2) - (this->help_info_[i]->GetTexture()->GetWidth() / 2);
        help_info_[i]->MoveTo({(float)w_center, static_cast<float>(300.0 + 20.0 * i)});
    }
}


} // namespace ui