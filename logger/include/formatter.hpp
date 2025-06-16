// formatter.hpp for logging formatter

#pragma once

#include "record.hpp"

#include <string>

class Formatter
{
public:
    Formatter() = default;

    virtual std::string format(const Record &record) = 0;
    virtual ~Formatter() = default;

    // -- disable copy/assign -- //
    Formatter(const Formatter &) = delete;
    Formatter& operator=(const Formatter &) = delete;

    // -- disable move/assign -- //
    Formatter(const Formatter &&) = delete;
    Formatter& operator=(const Formatter &&) = delete;
};

// 2025-05-16 11:17:23 - INFO - message
class BasicFormatter: public Formatter
{
public:
    std::string format(const Record &record) override;
};