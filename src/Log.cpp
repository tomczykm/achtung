#include "Log.hpp"
#include <ctime>

#include <fmt/core.h>

Logger::Logger(const std::string &filename) {
    file_.open(filename.c_str(), std::ios::trunc);
}

Logger::~Logger() {
    flush();
}

Logger::LogLevel& Logger::operator<<(Logger::LogLevel& level) {
    flush();
    buffer_ << std::time(nullptr) << ' ' << level.code_ << ": ";
    return level;
}

void Logger::flush() {
    file_ << buffer_.str() << std::endl;
    std::cout << buffer_.str() << std::endl;
    buffer_.str(std::string());
}

Logger log_;
Logger::LogLevel info(log_, "inf");
Logger::LogLevel warning(log_, "WRN");
Logger::LogLevel error(log_, "ERR");
