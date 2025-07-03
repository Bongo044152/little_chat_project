// logger.hpp ( main logger )
#pragma once

// -- Core Logging Components -- //
#include "formatter.hpp"  // Interface for log message formatting
#include "logLevel.hpp"   // Log severity level definitions
#include "record.hpp"     // Log record data structure
#include "sink.hpp"       // Base class for log output destinations

// -- Standard Library Containers -- //
#include <unordered_map>  // For logger name lookup table
#include <vector>         // For storing log sinks

// -- Thread Support Headers -- //
#include <chrono>              // std::chrono (for timestamp handling)
#include <condition_variable>  // std::condition_variable (for async logging)
#include <mutex>               // std::mutex (for thread synchronization)
#include <thread>              // std::thread (for potential async logging)

/**
 * @brief Thread-safe logger class that handles log message processing.
 *
 * Provides multi-level logging capabilities with configurable sinks and
 * formatters.
 *
 * NOTE: This class is not copyable or movable due to internal mutex.
 */
class Logger
{
public:
    // -- basic log -- //

    /**
     * @brief Log a message with specified level.
     *
     * @param lvl Log level for the message.
     * @param message The log message content.
     *
     * NOTE: Thread-safe operation protected by internal mutex.
     */
    void log(LogLevel lvl, const std::string &message);

    // -- quick log method -- //

    /**
     * @brief Log a debug level message.
     * @param message The debug message to log.
     */
    void debug(const std::string &message) { log(LogLevel::Debug, message); }

    /**
     * @brief Log an info level message.
     * @param message The informational message to log.
     */
    void info(const std::string &message) { log(LogLevel::Info, message); }

    /**
     * @brief Log a warning level message.
     * @param message The warning message to log.
     */
    void warning(const std::string &message)
    {
        log(LogLevel::Warning, message);
    }

    /**
     * @brief Log an error level message.
     * @param message The error message to log.
     */
    void error(const std::string &message) { log(LogLevel::Error, message); }

    /**
     * @brief Log a critical level message.
     * @param message The critical message to log.
     */
    void critical(const std::string &message)
    {
        log(LogLevel::Critical, message);
    }

    // -- setter -- //

    /**
     * @brief Add a sink to output log messages.
     * @param sink_ Pointer to the sink to add.
     */
    void addSink(Sink *sink_);

    /**
     * @brief Set the formatter for log records.
     * @param fmt_ Unique pointer to the formatter implementation.
     */
    void setFormatter(std::unique_ptr<Formatter> fmt_);

    /**
     * @brief Set the minimum log level for message processing.
     * @param lvl Minimum log level to filter messages.
     */
    void setLevel(LogLevel lvl);

    /**
     * @brief Set the minimum log level for message processing using an integer.
     *
     * This overload accepts an integer which will be interpreted as a log
     * level.
     *
     * @param lvl Integer value representing the minimum log level.
     *            Must be in the range [0, 4].
     *
     * @throws std::invalid_argument if lvl is outside the range [0, 4].
     */
    void setLevel(int lvl);

    // -- copy trait -- //

    Logger(const Logger &) = delete;
    Logger &operator=(const Logger &) = delete;

    // -- move trait -- //

    Logger(Logger &&) = delete;
    Logger &operator=(Logger &&) = delete;

    // -- destructor -- //

    ~Logger() = default;

    // friend for Register
    friend class LoggerRegistry;

private:
    // -- constructor -- //

    /**
     * @brief Private constructor (only accessible by LoggerRegistry).
     *
     * Initializes with default BasicFormatter and Debug log level.
     */
    Logger()
        : formatter_(std::make_unique<BasicFormatter>()), lvl_(LogLevel::Debug)
    {
    }

    // -- member variable -- //
    std::vector<Sink *> sinks_;             ///< Collection of log output sinks
    std::unique_ptr<Formatter> formatter_;  ///< Formatter for log records
    LogLevel lvl_;                          ///< Minimum log level filter
    std::mutex mtx_;                        ///< Mutex for thread safety

};  // end of Logger


/**
 * @brief Registry for managing named logger instances (Singleton).
 *
 * Provides centralized logger management with thread-safe access.
 * NOTE: This class is not copyable or movable.
 */
class LoggerRegistry
{
public:
    // -- singleton -- //

    /**
     * @brief Get the singleton instance of LoggerRegistry.
     * @return Reference to the single LoggerRegistry instance.
     */
    static LoggerRegistry &instance();

    // -- logger access -- //

    /**
     * @brief Get or create a named logger instance.
     * @param logger_name_ Name identifier for the logger.
     * @return Shared pointer to the requested logger.
     *
     * NOTE: Thread-safe operation. Creates new logger if name doesn't exist.
     */
    std::shared_ptr<Logger> get_logger(const std::string &logger_name_);

    // -- destructor -- //

    ~LoggerRegistry() = default;

    // -- copy trait -- //

    LoggerRegistry(const LoggerRegistry &) = delete;
    LoggerRegistry &operator=(const LoggerRegistry &) = delete;

    // -- move trait -- //

    LoggerRegistry(LoggerRegistry &&) = delete;
    LoggerRegistry &operator=(LoggerRegistry &&) = delete;

private:
    // -- private constructor -- //

    LoggerRegistry() = default;

    // -- member variable -- //
    std::unordered_map<std::string, std::shared_ptr<Logger>>
        LoggerTable_;  ///< Map of named logger instances
    std::mutex mtx_;   ///< Mutex for thread safety

};  // end of LoggerRegistry
