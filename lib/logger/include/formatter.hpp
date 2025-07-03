// formatter.hpp for logging formatter

#pragma once

#include <string>
#include "record.hpp"

/**
 * @brief Abstract base class for log record formatting.
 *
 * Provides an interface for converting log records into formatted strings.
 *
 * Derived classes must implement the format() method.
 *
 * NOTE: This class is not copyable or movable due to the virtual interface.
 */
class Formatter
{
public:
    Formatter() = default;

    /**
     * @brief Format a log record into a string representation.
     *
     * @param record The log record to be formatted.
     * @return std::string The formatted log message.
     */
    virtual std::string format(const Record &record) = 0;

    /**
     * @brief Virtual destructor for proper polymorphic destruction.
     */
    virtual ~Formatter() = default;

    // -- disable copy/assign -- //

    Formatter(const Formatter &) = delete;
    Formatter &operator=(const Formatter &) = delete;

    // -- disable move/assign -- //

    Formatter(const Formatter &&) = delete;
    Formatter &operator=(const Formatter &&) = delete;
};

/**
 * @brief Basic log formatter implementing standard format:
 *        "YYYY-MM-DD HH:MM:SS - LEVEL - message"
 *
 * Example output:
 * "2025-05-16 11:17:23 - INFO - Sample log message"
 */
class BasicFormatter : public Formatter
{
public:
    /**
     * @brief Format the log record into standard string representation.
     *
     * @param record The log record to format.
     * @return std::string Formatted message in "timestamp - level - message"
     * format.
     */
    std::string format(const Record &record) override;
};