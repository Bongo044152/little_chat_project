#include <iostream>

// -- headers for thread -- //
#include <thread>
#include <chrono>
#include <mutex>
#include <condition_variable>

#include "my_logger.hpp"

void fun1()
{
    try
    {
        auto logger = LoggerRegistry::instance().get_logger("fun1");
        logger->addSink(TerminalSink::get());
        logger->addSink(FileSink::get("logs/test.log"));
        logger->debug("大周是北七!");
        logger->debug("大周是北七!");
        logger->debug("大周是北七!");
    }
    catch(const std::exception& e)
    {
        std::cerr << e.what() << '\n';
    }
}

void fun2()
{
    try
    {
        auto logger = LoggerRegistry::instance().get_logger("fun2");
        logger->addSink(TerminalSink::get());
        logger->addSink(FileSink::get("logs/test.log"));
        logger->debug("李伯營是北七!");
        logger->debug("李伯營是北七!");
        logger->debug("李伯營是北七!");
    }
    catch(const std::exception& e)
    {
        std::cerr << e.what() << '\n';
    }
    
}

int main()
{
    std::thread thread_1(fun1);
    std::thread thread_2(fun2);

    if(thread_1.joinable())
        thread_1.join();

    if(thread_2.joinable())
        thread_2.join();

    return 0;
}