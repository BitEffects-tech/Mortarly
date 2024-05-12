#pragma once

#include "fmt/format.h"

namespace logging {
    enum LogLevel {
        Error = 0,
        Info = 1,
        Debug = 2
    };

    class Logger {
    public:
        virtual void write(const std::string &str) = 0;
    };

    extern std::vector<std::shared_ptr<Logger>> loggers;

    extern void setLogLevel(LogLevel level);
    extern void write(LogLevel level, const std::string& message);

    template<typename... Args>
    inline void error(const fmt::format_string<Args...> &fmt, Args... args) {
        write(LogLevel::Error, fmt::format(fmt, args...));
    }

    template<typename... Args>
    inline void info(const fmt::format_string<Args...> &fmt, Args... args) {
        write(LogLevel::Info, fmt::format(fmt, args...));
    }

    template<typename... Args>
    inline void debug(const fmt::format_string<Args...> &fmt, Args... args) {
        write(LogLevel::Debug, fmt::format(fmt, args...));
    }
}
