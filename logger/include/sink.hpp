// sink.hpp for logger

#pragma once

#include <string>
#include <fstream>
#include <mutex>
#include <unordered_map>
#include <memory>

#include <stdexcept>

// basic Sink

class Sink
{
public:
    Sink() = default;

    virtual void write(const std::string &message) = 0;
    virtual ~Sink() = default;

    // -- disable copy/assign -- //
    Sink(const Sink &) = delete;
    Sink& operator=(const Sink &) = delete;

    // -- disable move/assign -- //
    Sink(const Sink &&) = delete;
    Sink& operator=(const Sink &&) = delete;
};

// TerminalSink 

class TerminalSink: public Sink
{
public:
    static TerminalSink* get();
    void write(const std::string &message) override;
private:
    TerminalSink() = default;   // 禁止外部使用
    std::mutex mtx_;
};

// FileSink

class FileSink: public Sink
{
public:
    static FileSink* get(const std::string &filename);
    void write(const std::string &message) override;

    ~FileSink()
    {
        if(file_.is_open())
            file_.close();
    }
private:
    // 禁止外部建構
    explicit FileSink(const std::string &filename)
        : file_(filename, std::ios::app)  // append mode
    {
        if (!file_.is_open())
            throw std::runtime_error("Failed to open log file: " + filename);
    }

    std::ofstream file_;
    std::mutex mtx_;
};

// DailyFileSink

class DailyFileSink : public Sink
{
public:
    static DailyFileSink* get();
    void write(const std::string &message) override;

    ~DailyFileSink()
    {
        if (ofs_.is_open())
        ofs_.close();
    }
private:
    DailyFileSink() = default;      // 禁止外部建構
    std::string current_filename_;
    std::ofstream ofs_;
    std::mutex mtx_;
};