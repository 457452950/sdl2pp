#include "SWindow.hpp"

#include <thread>
#include <algorithm>

#include <fmt/format.h>


namespace sdlpp {

const PointI SWindow::DEFAULT_SIZE = {640, 480};

int SWindow::Exec() {
    active_.store(true);

    // first render
    this->render();

    last_physic_time_ = last_render_time_ = sdlpp::GetPerformanceCounter();

    int event_deal = max_event_deal_count_;

    // fixme: fps降低导致pollevent变慢
    while(active_) {
        // note: The order of judgment cannot be reversed, otherwise the event will be lost
        while(event_deal > 0 && SDL_PollEvent(&event_) == 1) {
            --event_deal;
            this->eventHandle(event_);
        }
        event_deal = max_event_deal_count_;

        if(!active_) {
            break;
        }

        auto cur = GetPerformanceCounter();

        this->CheckPhysicFrame(cur);
        this->CheckRenderFrame(cur);
    }

    return 0;
}

void SWindow::Close() { this->active_.store(false); }

void SWindow::DefaultWindowEvent(const SDL_WindowEvent &event) {
    switch(event.event) {
    case SDL_WINDOWEVENT_CLOSE: {
        LOG_INF(log::LIB, "SDL_WINDOW EVENT close.");
        this->Close();
        break;
    }
    }
}

uint64_t SWindow::CheckPhysicFrame(uint64_t current_time) {
    if(physic_delay_mics_ == 0) {
        return INT_MAX;
    }

    auto pass_time = (current_time - last_physic_time_);
    if(pass_time >= physic_delay_mics_) {
        this->Tick((double)pass_time / (double)sdlpp::GetPerformanceFrequency() * 1000.0);
        last_physic_time_ += pass_time;
        return physic_delay_mics_;
    } else {
        return physic_delay_mics_ - pass_time;
    }
}

uint64_t SWindow::CheckRenderFrame(uint64_t current_time) {
    if(render_delay_mics_ == 0) {
        this->render();
        return 0;
    }

    auto pass_time = current_time - last_render_time_;
    if(pass_time >= render_delay_mics_) {
        this->render();
        last_render_time_ += pass_time;
        return render_delay_mics_;
    } else {
        return render_delay_mics_ - pass_time;
    }
}

void SWindow::eventHandle(const SDL_Event &event) {
    switch(static_cast<SDL_EventType>(event_.type)) {
    case SDL_QUIT: {
        LOG_INF(log::LIB, "SDL_QUIT");
        break;
    }
    case SDL_DISPLAYEVENT: {
        auto &ev = event_.display;
        switch(static_cast<SDL_DisplayEventID>(ev.event)) {
        case SDL_DISPLAYEVENT_NONE:
            LOG_INF(log::LIB, "display event: none index {}", ev.display);
            break;
        case SDL_DISPLAYEVENT_ORIENTATION:
            LOG_INF(log::LIB, "display event: orientation {} change to {}", ev.display, ev.data1);
            break;
        case SDL_DISPLAYEVENT_CONNECTED:
            LOG_INF(log::LIB, "display event: connected {}", ev.display);
            break;
        case SDL_DISPLAYEVENT_DISCONNECTED:
            LOG_INF(log::LIB, "display event: disconnected {}", ev.display);
            break;
        }
        break;
    }
    case SDL_KEYUP:
    case SDL_KEYDOWN: {
        if(event_.key.windowID == this->GetID())
            this->KeyEvent(event_.key);
        break;
    }
    case SDL_MOUSEMOTION: {
        if(event_.motion.windowID == this->GetID())
            this->MouseMoveEvent(event_.motion);
        break;
    }
    case SDL_MOUSEBUTTONUP:
    case SDL_MOUSEBUTTONDOWN: {
        if(event_.button.windowID == this->GetID())
            this->MouseButtonEvent(event_.button);
        break;
    }
    case SDL_MOUSEWHEEL: {
        if(event_.wheel.windowID == this->GetID())
            this->MouseWheelEvent(event_.wheel);
        break;
    }
    case SDL_WINDOWEVENT: {
        if(event_.key.windowID == this->GetID()) {
            auto res = this->WindowEvent(event_.window);
            if(res == Continue) {
                DefaultWindowEvent(event_.window);
            }
        }
        break;
    }
    default:
        break;
    }
}

} // namespace sdlpp