// impl for sink.hpp

#include <ctime>    // std::time_t, std::localtime
#include <iomanip>  // std::put_time
#include <iostream>
#include <sstream>

#include <fstream>

#include "sink.hpp"

// TerminalSink

void TerminalSink::write(const std::string &message)
{
    std::lock_guard<std::mutex> lock(mtx_);
    std::cout << message << "\n";
}

TerminalSink *TerminalSink::get()
{
    static TerminalSink ts;
    return &ts;
}

// FileSink

void FileSink::write(const std::string &message)
{
    std::lock_guard<std::mutex> lock(mtx_);
    if (file_.is_open())
        file_ << message << "\n";
}

FileSink *FileSink::get(const std::string &filename)
{
    static std::mutex mtx_;
    static std::unordered_map<std::string, std::unique_ptr<FileSink>> m;

    std::lock_guard<std::mutex> lock(mtx_);
    if (m.find(filename) == m.end()) {
        m[filename] =
            std::move(std::unique_ptr<FileSink>(new FileSink(filename)));
    }
    return m.at(filename).get();
}

// DailyFileSink

static std::string _get_today_filename()
{
    std::time_t t = std::time(nullptr);
    std::tm tm = *std::localtime(&t);
    std::ostringstream oss;
    oss << std::put_time(&tm, "%Y_%m_%d") << ".log";
    return oss.str();  // e.g. "2025_06_14.log"
}

void DailyFileSink::write(const std::string &message)
{
    std::lock_guard<std::mutex> lock(mtx_);
    std::string today_filename = "logs/" + _get_today_filename();
    if (today_filename != current_filename_) {
        // 日期變了，重新開檔案
        if (ofs_.is_open())
            ofs_.close();

        current_filename_ = today_filename;
        ofs_.open(current_filename_, std::ios::app);  // append mod
    }

    if (ofs_.is_open())
        ofs_ << message << "\n";
    else {
        std::stringstream ss;
        ss << "Failed to open log file: " << today_filename;
        throw std::runtime_error(ss.str());
    }
}

DailyFileSink *DailyFileSink::get()
{
    static DailyFileSink d;
    return &d;
}