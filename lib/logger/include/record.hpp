// record.hpp : record unit for logging
#pragma once

#include "logLevel.hpp"  // Log level enumeration definitions

#include <chrono>       // For time_point and system_clock
#include <thread>       // For thread::id operations
#include <string>       // For message string storage

/**
 * @brief Log record data structure
 *
 * Encapsulates all metadata and content for a single log entry.
 * 
 * All members are const to ensure immutability after creation.
 * 
 * This structure flows through the entire logging pipeline.
 */
struct Record
{
    const std::chrono::system_clock::time_point recorded_time;  ///< Timestamp when log was created
    const std::string message;                                  ///< The actual log message content
    const LogLevel lvl;                                         ///< Severity level of the log
    const std::thread::id thread_id;                            ///< ID of thread that generated the log

    /**
     * @brief Constructs a complete log record
     * @param t Timestamp (typically std::chrono::system_clock::now())
     * @param message The log message content
     * @param level Severity level from LogLevel enum
     * @param id Thread ID (typically std::this_thread::get_id())
     */
    Record(std::chrono::system_clock::time_point t,
        const std::string &message,
        LogLevel level,
        std::thread::id id)
        : recorded_time(t), 
        message(message), 
        lvl(level), 
        thread_id(id) 
    {}
};