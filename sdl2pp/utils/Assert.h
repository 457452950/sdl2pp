#ifndef SDL2PP_SDL2PP_UTILS_ASSERT_H_
#define SDL2PP_SDL2PP_UTILS_ASSERT_H_

#include <cassert>

#include <fmt/printf.h>

#define Assert(expression, ...)                                                                                        \
    if(!(expression)) {                                                                                                \
        fmt::print(__VA_ARGS__);                                                                                       \
        std::abort();                                                                                                  \
    }


#endif // SDL2PP_SDL2PP_UTILS_ASSERT_H_
