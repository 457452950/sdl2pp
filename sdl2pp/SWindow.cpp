#include "SWindow.h"


#include <fmt/format.h>

namespace sdlpp {

    const SDL_Point SWindow::DEFAULT_SIZE = {640, 480};

    int SWindow::Exec() {
        active_.store(true);

        while (active_) {
            SDL_PollEvent(&event_);

            switch (event_.type) {
                case SDL_QUIT: {
                    SDL_LogDebug(0, "SDL_QUIT");
                    break;
                }
                case SDL_KEYDOWN: {
                    if (event_.key.windowID == this->GetID())
                        this->KeyEvent(event_.key);
                    break;
                }
                case SDL_MOUSEMOTION: {
                    if (event_.motion.windowID == this->GetID())
                        SDL_Log("mouse motion state %d x %d y %d  xrel %d yrel %d",
                                event_.motion.state, event_.motion.x, event_.motion.y, event_.motion.xrel,
                                event_.motion.yrel);
                    break;
                }
                case SDL_MOUSEBUTTONDOWN : {
                    if (event_.button.windowID == this->GetID())
                        SDL_Log("mouse button down state %d x %d y %d button %d click %d",
                                event_.button.state, event_.button.x, event_.button.y, event_.button.button,
                                event_.button.clicks);
                    break;
                }
                case SDL_WINDOWEVENT: {
                    if (event_.key.windowID == this->GetID()) {
                        auto res = this->WindowEvent(event_.window);
                        if (res == Continue) {
                            DefaultWindowEvent(event_.window);
                        }
                    }
                    break;
                }
            }


            SDL_RenderClear(renderer_);
            this->RenderProcess();
            SDL_RenderPresent(renderer_);
        }

        return 0;
    }

    void SWindow::Close() {
        this->active_.store(false);
    }

    void SWindow::DefaultWindowEvent(const SDL_WindowEvent &event) {
        switch (event.event) {
            case SDL_WINDOWEVENT_CLOSE: {
                SDL_LogDebug(0, "SDL_WINDOW EVENT close");
                this->Close();
                break;
            }
        }
    }

} // sdlpp