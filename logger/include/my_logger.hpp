// logger.hpp ( main logger )

#pragma once

#include "logLevel.hpp"
#include "sink.hpp"
#include "formatter.hpp"
#include "record.hpp"

#include <vector>
#include <unordered_map>

// -- headers for thread -- //
#include <thread>
#include <chrono>
#include <mutex>
#include <condition_variable>

class Logger
{
public:
    // -- basic log -- //

    void log(LogLevel lvl, const std::string &message);

    // -- quick log method -- //

    void debug(const std::string &message) { log(LogLevel::Debug, message); }
    void info(const std::string &message) { log(LogLevel::Info, message); }
    void warning(const std::string &message) { log(LogLevel::Warning, message); }
    void error(const std::string &message) { log(LogLevel::Error, message); }
    void critical(const std::string &message) { log(LogLevel::Critical, message); }

    // -- setter -- //

    void addSink(Sink *sink_);
    
    void setFormatter(std::unique_ptr<Formatter> fmt_);

    void setLevel(LogLevel lvl);

    // -- copy trait -- //
    Logger(const Logger &) = delete;
    Logger& operator=(const Logger &) = delete;

    // -- move trait -- //
    Logger(Logger &&) = delete;
    Logger& operator=(Logger &&) = delete;

    // -- detructor -- //
    ~Logger() = default;

    // firend for Register
    friend class LoggerRegistry;

private:
    // -- constructor -- //
    Logger() : formatter_(std::make_unique<BasicFormatter>()), lvl_(LogLevel::Debug) {}

    // -- member variable -- //
    std::vector<Sink*> sinks_;  // sinks
    std::unique_ptr<Formatter> formatter_;        // formatter
    LogLevel lvl_;      // level for Logger
    std::mutex mtx_;    // avoid race condition

}; // end of Logger


class LoggerRegistry
{
public:
    // -- singleton -- //
    static LoggerRegistry& instance();

    // -- member variable -- //
    std::shared_ptr<Logger> get_logger(const std::string &logger_name_);

    // -- destructor -- //
    ~LoggerRegistry() = default;

    // -- copy trait -- //
    LoggerRegistry(const LoggerRegistry &) = delete;
    LoggerRegistry& operator=(const LoggerRegistry &) = delete;
    
    // -- move trait -- //
    LoggerRegistry(LoggerRegistry &&) = delete;
    LoggerRegistry& operator=(LoggerRegistry &&) = delete;

private:
    // -- private constructor -- //
    LoggerRegistry() = default;

    // -- member variable -- //
    std::unordered_map<std::string, std::shared_ptr<Logger>> LoggerTable_;  // string to find logger
    std::mutex mtx_;        // to avoid race condition

}; // end of LoggerRegistry