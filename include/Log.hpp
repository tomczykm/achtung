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
        LogLevel& operator<<(const T& t) {
            log_.buffer_ << t;
        }

        Logger& log_;
        const std::string code_;
    };

    explicit Logger(const std::string &filename = "event.log");
    ~Logger();

    LogLevel& operator<<(LogLevel& level);

private:
    void flush() {
        file_ << buffer_.str() << std::endl;
        std::cout << buffer_.str() << std::endl;
        buffer_.str(std::string());
    }
    std::ofstream file_;
    std::stringstream buffer_;
};

extern Logger log_;
extern Logger::LogLevel info;
extern Logger::LogLevel warning;
extern Logger::LogLevel error;
