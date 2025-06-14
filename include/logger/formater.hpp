// formater.hpp for logging formater

#pragma once

#include "record.hpp"

#include <string>

class Formater
{
public:
    Formater() = default;

    virtual std::string format(const Record &record) = 0;
    virtual ~Formater() = default;

    // -- disable copy/assign -- //
    Formater(const Formater &) = delete;
    Formater& operator=(const Formater &) = delete;

    // -- disable move/assign -- //
    Formater(const Formater &&) = delete;
    Formater& operator=(const Formater &&) = delete;
};

// 2025-05-16 11:17:23 - INFO - message
class BasicFormater: public Formater
{
public:
    std::string format(const Record &record) override;
};