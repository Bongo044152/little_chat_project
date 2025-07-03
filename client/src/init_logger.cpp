/**
 * @file init_logger.cpp
 * @brief Initializes and configures the global logging system.
 */

#include "logger.hpp"

#ifndef LOG_LEVEL
#error "LOG_LEVEL is needed"
#endif

#define SET_LOG_LEVEL(logger) ((logger)->setLevel((LOG_LEVEL)))

/**
 * @brief Initializes the global logger.
 *
 * @note This function should be called exactly once, ideally at the start of
 * main().
 * @note Subsequent calls will have no effect.
 */
void init_logger()
{
    // Prevent multiple initializations
    static bool called = false;
    if (called)
        return;
    called = true;

    // TODO: init logger here
}
