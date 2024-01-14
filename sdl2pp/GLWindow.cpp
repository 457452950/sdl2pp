#include "GLWindow.hpp"

namespace sdlpp {

GlWindow::GlWindow() : sdlpp::SWindow(SDL_WINDOW_OPENGL) {}

void GlWindow::RenderFlush() { SDL_GL_SwapWindow(this->Get()); }


} // namespace sdlpp