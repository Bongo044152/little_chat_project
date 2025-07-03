// impl for my_logger.hpp

#include <iostream>
#include <stdexcept>
#include <string>

#include "my_logger.hpp"

// Logger

void Logger::log(LogLevel lvl, const std::string &message)
{
    std::lock_guard<std::mutex> lock(mtx_);
    if (!formatter_ || sinks_.empty())
        throw std::runtime_error("formatter and sink are needed!");
    if (lvl < lvl_)
        return;

    Record record(std::chrono::system_clock::now(), message, lvl,
                  std::this_thread::get_id());

    const std::string formatted = formatter_->format(record);
    for (const auto &it : sinks_) {
        it->write(formatted);
    }
}

void Logger::addSink(Sink *sink_)
{
    std::lock_guard<std::mutex> lock(mtx_);
    sinks_.push_back(sink_);
}

void Logger::setFormatter(std::unique_ptr<Formatter> fmt_)
{
    std::lock_guard<std::mutex> lock(mtx_);
    formatter_ = std::move(fmt_);
}

void Logger::setLevel(LogLevel lvl)
{
    std::lock_guard<std::mutex> lock(mtx_);
    lvl_ = lvl;
}

void Logger::setLevel(int lvl)
{
    std::lock_guard<std::mutex> lock(mtx_);
    switch (lvl) {
    case 0:
        lvl_ = LogLevel::Debug;
        break;
    case 1:
        lvl_ = LogLevel::Info;
        break;
    case 2:
        lvl_ = LogLevel::Warning;
        break;
    case 3:
        lvl_ = LogLevel::Error;
        break;
    case 4:
        lvl_ = LogLevel::Critical;
        break;
    default:
        throw std::invalid_argument("0 <= lvl <= 4");
        break;
    }
}

// LoggerRegistry

LoggerRegistry &LoggerRegistry::instance()
{
    static LoggerRegistry registry;
    return registry;
}

std::shared_ptr<Logger> LoggerRegistry::get_logger(
    const std::string &logger_name_)
{
    std::lock_guard<std::mutex> lock(mtx_);
    if (LoggerTable_.find(logger_name_) != LoggerTable_.end()) {
        return LoggerTable_.at(logger_name_);
    } else {
        LoggerTable_[logger_name_] =
            std::move(std::shared_ptr<Logger>(new Logger()));
        return LoggerTable_.at(logger_name_);
    }
}