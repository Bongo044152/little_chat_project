// logger.hpp ( main logger )

#pragma once

#include "logLevel.hpp"
#include "sink.hpp"
#include "formater.hpp"
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

    void debug(const std::string &message) { log(LogLevel::DEBUG, message); }
    void info(const std::string &message) { log(LogLevel::INFO, message); }
    void warning(const std::string &message) { log(LogLevel::WARNING, message); }
    void error(const std::string &message) { log(LogLevel::ERROR, message); }
    void critical(const std::string &message) { log(LogLevel::CRITICAL, message); }

    // -- setter -- //

    void addSink(Sink *sink_);
    
    void setFormater(std::unique_ptr<Formater> fmt_);

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
    Logger() : formater_(std::make_unique<BasicFormater>()), lvl_(LogLevel::DEBUG) {}

    // -- member variable -- //
    std::vector<Sink*> sinks_;  // sinks
    std::unique_ptr<Formater> formater_;        // formater
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