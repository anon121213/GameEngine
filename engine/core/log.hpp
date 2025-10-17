#pragma once
#include <string>
#include <iostream>
#include <sstream>
#include <format>
#include <windows.h>

namespace Log {

    enum class Level {
        Trace,
        Info,
        Warn,
        Error
    };

    inline auto currentLevel = Level::Trace;
    inline void SetLevel(const Level level) { currentLevel = level; }

    template<typename... Args>
    void Print(Level level, std::string_view fmt, Args&&... args)
    {
        if (level < currentLevel)
            return;

        std::string prefix;
        std::string colorCode;

        switch (level) {
            case Level::Trace: prefix = "[TRACE]"; colorCode = "\033[90m"; break;
            case Level::Info:  prefix = "[INFO] "; colorCode = "\033[36m"; break;
            case Level::Warn:  prefix = "[WARN] "; colorCode = "\033[33m"; break;
            case Level::Error: prefix = "[ERROR]"; colorCode = "\033[31m"; break;
        }

        std::string formatted;
        try {
            formatted = std::vformat(fmt, std::make_format_args(args...));
        }
        catch (...) {
            std::ostringstream oss;
            (oss << ... << args);
            formatted = fmt.data();
            formatted += " ";
            formatted += oss.str();
        }

        std::string output = prefix + " " + formatted + "\n";

        std::cout << colorCode << output << "\033[0m";
        OutputDebugStringA(output.c_str());
    }

} 

#define LOG_TRACE(...) Log::Print(Log::Level::Trace, __VA_ARGS__)
#define LOG_INFO(...)  Log::Print(Log::Level::Info,  __VA_ARGS__)
#define LOG_WARN(...)  Log::Print(Log::Level::Warn,  __VA_ARGS__)
#define LOG_ERROR(...) Log::Print(Log::Level::Error, __VA_ARGS__)
