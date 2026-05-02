#pragma once

#include <iostream>
#include <chrono>
#include <iomanip>
#include <string>


class Logger
{
public:
    static Logger& instance();
    void info(const std::string& msg);
    void error(const std::string& msg);
    void debug(const std::string& msg);
private:
    void log(const std::string& level, const std::string& msg);
    std::string timestamp();
};