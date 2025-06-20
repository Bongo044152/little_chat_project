// sink.hpp for logger
#pragma once

#include <string>         // For string handling
#include <fstream>        // For file operations
#include <mutex>          // For thread synchronization
#include <unordered_map>  // For filename mapping
#include <memory>         // For smart pointers
#include <stdexcept>      // For exception handling

/**
 * @brief Abstract base class for all log sinks
 * 
 * Defines the interface for writing log messages to various outputs.
 * 
 * All concrete sink implementations must override the write() method.
 */
class Sink
{
public:
    // -- constructor -- //

    Sink() = default;

    /**
     * @brief Pure virtual function for writing log messages
     * @param message The log message to be written
     */
    virtual void write(const std::string &message) = 0;

    // -- destructor -- //

    virtual ~Sink() = default;

    // -- disable copy/assign -- //
    Sink(const Sink &) = delete;               ///< Copy constructor (deleted)
    Sink& operator=(const Sink &) = delete;    ///< Copy assignment (deleted)

    // -- disable move/assign -- //
    Sink(Sink &&) = delete;                    ///< Move constructor (deleted)
    Sink& operator=(Sink &&) = delete;        ///< Move assignment (deleted)
};

/**
 * @brief Sink implementation for terminal/console output
 * 
 * Thread-safe singleton that writes log messages to standard output. ( output to the terminal )
 */
class TerminalSink: public Sink
{
public:
    /**
     * @brief Get the singleton instance
     * @return Pointer to the TerminalSink instance
     */
    static TerminalSink* get();
    
    /**
     * @brief Write a message to terminal
     * @param message The log message to output
     */
    void write(const std::string &message) override;

private:
    TerminalSink() = default;   ///< Private constructor for no external construction
    std::mutex mtx_;            ///< Mutex for thread safety
};

/**
 * @brief Sink implementation for file output
 * 
 * Thread-safe file logging with singleton instances per filename.
 */
class FileSink: public Sink
{
public:
    /**
     * @brief Get or create a FileSink instance for specific filename
     * @param filename The log file path
     * @return Pointer to the FileSink instance
     * @throws std::runtime_error if file cannot be opened
     */
    static FileSink* get(const std::string &filename);
    
    /**
     * @brief Write a message to the log file
     * @param message The log message to write
     */
    void write(const std::string &message) override;

    /**
     * @brief Destructor that ensures file is closed
     */
    ~FileSink()
    {
        if(file_.is_open())
            file_.close();
    }

private:
    /**
     * @brief Construct a new FileSink for specific filename
     * @param filename The log file path (opens in append mode)
     * @throws std::runtime_error if file cannot be opened
     */
    explicit FileSink(const std::string &filename)
        : file_(filename, std::ios::app)  // append mode
    {
        if (!file_.is_open())
            throw std::runtime_error("Failed to open log file: " + filename);
    }

    std::ofstream file_;  ///< Output file stream
    std::mutex mtx_;      ///< Mutex for thread safety
};

/**
 * @brief Sink implementation for daily rotating log files
 * 
 * Automatically creates new log files based on date.
 */
class DailyFileSink : public Sink
{
public:
    /**
     * @brief Get the singleton instance
     * @return Pointer to the DailyFileSink instance
     */
    static DailyFileSink* get();
    
    /**
     * @brief Write a message to current daily log file
     * @param message The log message to write
     */
    void write(const std::string &message) override;

    /**
     * @brief Destructor that ensures file is closed
     */
    ~DailyFileSink()
    {
        if (ofs_.is_open())
            ofs_.close();
    }

private:
    DailyFileSink() = default;      ///< Private constructor for singleton pattern
    std::string current_filename_;  ///< Current log filename (date-based)
    std::ofstream ofs_;             ///< Output file stream
    std::mutex mtx_;                ///< Mutex for thread safety
};