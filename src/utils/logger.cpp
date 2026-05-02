#include "../../include/utils/logger.h"

Logger& Logger::instance()
{
    static Logger instance;
    return instance;
}

void Logger::info(const std::string& msg)
{
    log("INFO", msg);
}

void Logger::debug(const std::string& msg)
{
    log("DEBUG", msg);
}

void Logger::error(const std::string& msg)
{
    log("ERROR", msg);
}

void Logger::log(const std::string& level, const std::string& msg)
{
    std::cout << "[" << timestamp() << "] "
                  << "[" << level << "] "
                  << msg << std::endl;
}

std::string Logger::timestamp()
{
    auto now = std::chrono::system_clock::now();
    auto time = std::chrono::system_clock::to_time_t(now);

    std::ostringstream oss;
    oss << std::put_time(std::localtime(&time), "%H:%M:%S");
    return oss.str();
}