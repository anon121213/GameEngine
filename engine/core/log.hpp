#pragma once
#include <string>
#include <iostream>
#include <format>

namespace Log {
    enum class Level {
        Trace,
        Info,
        Warn,
        Error
    };

    void SetLevel(Level level);
    inline Level currentLevel = Level::Trace;

    template<typename... Args>
    void Print(Level level, std::string_view formatStr, Args&&... args) {
        if (level < currentLevel) return;

        std::string prefix;
        std::string colorCode;

        switch (level) {
            case Level::Trace: prefix = "[TRACE]"; colorCode = "\033[90m"; break;
            case Level::Info:  prefix = "[INFO] "; colorCode = "\033[36m"; break;
            case Level::Warn:  prefix = "[WARN] "; colorCode = "\033[33m"; break;
            case Level::Error: prefix = "[ERROR]"; colorCode = "\033[31m"; break;
        }

        std::string formatted = std::vformat(formatStr, std::make_format_args(std::forward<Args>(args)...));
        std::cout << colorCode << prefix << " " << formatted << "\033[0m" << std::endl;
    }

#define LOG_TRACE(...) Log::Print(Log::Level::Trace, __VA_ARGS__)
#define LOG_INFO(...)  Log::Print(Log::Level::Info, __VA_ARGS__)
#define LOG_WARN(...)  Log::Print(Log::Level::Warn, __VA_ARGS__)
#define LOG_ERROR(...) Log::Print(Log::Level::Error, __VA_ARGS__)
}
