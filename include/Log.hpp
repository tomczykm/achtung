#pragma once

#include <string>
#include <fstream>
#include <iostream>
#include <sstream>

class Logger {
public:
    struct LogLevel {
        LogLevel(Logger &log, const std::string &code):
            log_(log),
            code_(code)
        {}

        template<typename T>
        LogLevel& operator<<(T&& t) {
            log_.buffer_ << std::forward<T>(t);
            return *this;
        }

        Logger& log_;
        const std::string code_;
    };

    explicit Logger(const std::string &filename = "event.log");
    ~Logger();

    LogLevel& operator<<(LogLevel& level);

private:
    void flush();

    std::ofstream file_;
    std::stringstream buffer_;
};

extern Logger log_;
extern Logger::LogLevel info;
extern Logger::LogLevel warning;
extern Logger::LogLevel error;
