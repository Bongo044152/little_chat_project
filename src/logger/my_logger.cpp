// impl for my_logger.hpp

#include <stdexcept>
#include <string>
#include <iostream>

#include "my_logger.hpp"

// Logger

void Logger::log(LogLevel lvl, const std::string &message)
{
    std::lock_guard<std::mutex> lock(mtx_);
    if(!formater_ || sinks_.empty()) throw std::runtime_error("formater and sink are needed!");
    if(lvl < lvl_) return;

    Record record(
        std::chrono::system_clock::now(),
        message,
        lvl,
        std::this_thread::get_id()
    );

    const std::string formatted = formater_->format(record);
    for(const auto &it : sinks_) {
        it->write(formatted);
    }
}

void Logger::addSink(Sink *sink_)
{
    std::lock_guard<std::mutex> lock(mtx_);
    sinks_.push_back(sink_);
}

void Logger::setFormater(std::unique_ptr<Formater> fmt_)
{
    std::lock_guard<std::mutex> lock(mtx_);
    formater_ = std::move(fmt_);
}

void Logger::setLevel(LogLevel lvl)
{
    std::lock_guard<std::mutex> lock(mtx_);
    lvl_ = lvl;
}

// LoggerRegistry

LoggerRegistry &LoggerRegistry::instance()
{
    static LoggerRegistry registry;
    return registry;
}

std::shared_ptr<Logger> LoggerRegistry::get_logger(const std::string &logger_name_)
{
    std::lock_guard<std::mutex> lock(mtx_);
    if(LoggerTable_.find(logger_name_) != LoggerTable_.end()) {
        return LoggerTable_.at(logger_name_);
    }
    else {
        LoggerTable_[logger_name_] = std::move(std::shared_ptr<Logger>(new Logger()));
        return LoggerTable_.at(logger_name_);
    }
}