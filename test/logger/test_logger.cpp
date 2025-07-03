// test logger
#define DOCTEST_CONFIG_IMPLEMENT_WITH_MAIN
#include "doctest.h"

#include "logger.hpp"

TEST_CASE("Test Doctest") {
    auto logger = LoggerRegistry::instance().get_logger("log");
    logger->setLevel(LogLevel::Debug);
    logger->addSink(TerminalSink::get());
    logger->info("你好!");
}