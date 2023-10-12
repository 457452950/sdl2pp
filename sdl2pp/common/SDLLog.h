#pragma once
#ifndef SDL2PP_BASE_SDLLOG_H_
#define SDL2PP_BASE_SDLLOG_H_

#ifdef WIN32
#include <Windows.h>
#endif
#include <varargs.h>
#include <thread>
#include <chrono>
#include <ctime>

#include <SDL_log.h>
#include <SDL_assert.h>

#include <fmt/format.h>
#include <fmt/ostream.h>
#include <fmt/chrono.h>

namespace sdlpp {

static auto getLocalThreadId() { return fmt::streamed(std::this_thread::get_id()); }
static auto getNowTime() {
#ifdef WIN32
    SYSTEMTIME t1;
    GetLocalTime(&t1);
    return fmt::format("{:%Y-%m-%d %T}:{:03}", fmt::localtime(std::time(nullptr)), t1.wMilliseconds);
#elif defined(__LINUX__)
    return "";
#endif
}

namespace log {
enum Category { APP = 0, LIB = SDL_LOG_CATEGORY_CUSTOM };
}
static std::string_view getLogCategoryName(log::Category category) {
    switch(category) {
    case log::APP:
        return "Application";
        break;
    case log::LIB:
        return "Library";
        break;
    }
    SDL_assert(false);
    return "";
}

static std::string MakeHead(log::Category tag, std::string_view file_name, int line_no, const char *func_name) {
    return fmt::format("[{}][{}][{}][{}:{}:{}]",
                       getNowTime(),
                       getLocalThreadId(),
                       getLogCategoryName(tag),
                       file_name,
                       line_no,
                       func_name);
}

#define LOG_VER(tag, fmt_s, ...)                                                                                       \
    SDL_LogMessage(                                                                                                    \
            sdlpp::tag,                                                                                                \
            SDL_LOG_PRIORITY_VERBOSE,                                                                                  \
            (sdlpp::MakeHead(sdlpp::tag, __FILE__, __LINE__, __func__) + fmt::format(fmt_s, ##__VA_ARGS__)).c_str())

#define LOG_DBG(tag, fmt_s, ...)                                                                                       \
    SDL_LogMessage(                                                                                                    \
            sdlpp::tag,                                                                                                \
            SDL_LOG_PRIORITY_DEBUG,                                                                                    \
            (sdlpp::MakeHead(sdlpp::tag, __FILE__, __LINE__, __func__) + fmt::format(fmt_s, ##__VA_ARGS__)).c_str())


#define LOG_INF(tag, fmt_s, ...)                                                                                       \
    SDL_LogMessage(                                                                                                    \
            sdlpp::tag,                                                                                                \
            SDL_LOG_PRIORITY_INFO,                                                                                     \
            (sdlpp::MakeHead(sdlpp::tag, __FILE__, __LINE__, __func__) + fmt::format(fmt_s, ##__VA_ARGS__)).c_str())

#define LOG_WRN(tag, fmt_s, ...)                                                                                       \
    SDL_LogMessage(                                                                                                    \
            sdlpp::tag,                                                                                                \
            SDL_LOG_PRIORITY_WARN,                                                                                     \
            (sdlpp::MakeHead(sdlpp::tag, __FILE__, __LINE__, __func__) + fmt::format(fmt_s, ##__VA_ARGS__)).c_str())

#define LOG_ERR(tag, fmt_s, ...)                                                                                       \
    SDL_LogMessage(                                                                                                    \
            sdlpp::tag,                                                                                                \
            SDL_LOG_PRIORITY_ERROR,                                                                                    \
            (sdlpp::MakeHead(sdlpp::tag, __FILE__, __LINE__, __func__) + fmt::format(fmt_s, ##__VA_ARGS__)).c_str())

#define LOG_CRI(tag, fmt_s, ...)                                                                                       \
    SDL_LogMessage(                                                                                                    \
            sdlpp::tag,                                                                                                \
            SDL_LOG_PRIORITY_CRITICAL,                                                                                 \
            (sdlpp::MakeHead(sdlpp::tag, __FILE__, __LINE__, __func__) + fmt::format(fmt_s, ##__VA_ARGS__)).c_str())

} // namespace sdlpp

#endif // SDL2PP_BASE_SDLLOG_H_
