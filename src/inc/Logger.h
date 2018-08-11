#pragma once

#include <string>
#include <fstream>
#include <iostream>

class Logger {
public:
    Logger(const std::string &filename = "event.log");
    ~Logger();

    void out(const std::string &o);
    void error(const std::string &o);
private:
    std::ofstream file_;
};
