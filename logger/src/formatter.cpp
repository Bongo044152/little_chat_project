// impl for formatter.hpp

#include "formatter.hpp"

#include <string>
#include <sstream>

#include <ctime>        // std::time_t, std::localtime
#include <iomanip>      // std::put_time

// -- helper function -- //

/**
 * @brief Convert LogLevel enum to string representation.
 * 
 * @param lev The log level to convert.
 * @return std::string The corresponding string.
 */
static std::string _LogLevel_to_string(LogLevel lev)
{
    switch (lev)
    {
    case LogLevel::DEBUG:
        return "DEBUG";
    case LogLevel::INFO:
        return "INFO";
    case LogLevel::WARNING:
        return "WARNING";
    case LogLevel::ERROR:
        return "ERROR";
    case LogLevel::CRITICAL:
        return "CRITICAL";
    default:
        return "UNKNOWN";
    }
}

// BasicFormatter

std::string BasicFormatter::format(const Record &record)
{
    using std::chrono::system_clock;
    std::time_t now_c = system_clock::to_time_t(record.recorded_time);
    std::stringstream oss;
    oss << std::put_time(std::localtime(&now_c), "%Y-%m-%d %H:%M:%S") << " - " << _LogLevel_to_string(record.lvl)
        << " - " << record.message;
    return oss.str();
}

// ...