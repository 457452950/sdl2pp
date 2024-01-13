#include "Point.hpp"

namespace sdlpp {
PointF::PointF(const PointD &other) : SDL_FPoint({static_cast<float>(other.x), static_cast<float>(other.y)}) {}
} // namespace sdlpp