#include "Math.h"

namespace sdlpp {

double Atan(double y, double x) {

#ifdef WIN32
    return std::atan2f(static_cast<float>(y), static_cast<float>(x));
#elif defined(__LINUX__)
    if(position_.x != 0) {
        return std::atan(position_.y / position_.x);
    } else {
        return M_PI / 2;
    }
#endif
}

} // namespace sdlpp