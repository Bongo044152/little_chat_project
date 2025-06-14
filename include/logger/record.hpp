// record.hpp : record unit for logging

#include "logLevel.hpp"

#include <chrono>
#include <thread>
#include <string>


#pragma once

struct Record
{
    const std::chrono::system_clock::time_point recorded_time;    ///< recorded time
    const std::string message;                                    ///< message from user
    const LogLevel lvl;                                           ///< log level
    const std::thread::id thread_id;                              ///< thread id (from which thread)

    Record(std::chrono::system_clock::time_point t,
        const std::string &msg,
        LogLevel level,
        std::thread::id id)
        : recorded_time(t), message(msg), lvl(level), thread_id(id) {}
};
