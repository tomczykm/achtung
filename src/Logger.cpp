#include "Logger.h"
#include <ctime>

Logger::Logger(const std::string &filename) {
    file_.open(filename.c_str(), std::ios::trunc);
}

Logger::~Logger() {
    out("End log.");
    file_.close();
}

void Logger::out(const std::string &o) {
    file_ << std::time(nullptr) << ' ' << o << '\n';
    std::cout << o << '\n';
}

void Logger::error(const std::string &o) {
    file_ << std::time(nullptr) << ' ' << o << std::endl;
    std::cerr << o << std::endl;
}
