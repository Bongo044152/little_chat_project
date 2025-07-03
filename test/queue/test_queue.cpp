// test queue

#define DOCTEST_CONFIG_IMPLEMENT_WITH_MAIN
#include "doctest.h"

// -- headers for thread -- //
#include <chrono>
#include <condition_variable>
#include <mutex>
#include <thread>


#include <array>
#include <iostream>

// -- Queue for MT-safe -- //
#include "thread_safe_queue.hpp"

TEST_CASE("Basic Queue")
{
    Queue<int> q;
    std::array<int, 3> arr = {1, 2, 3};

    SUBCASE("test push")
    {
        try {
            q.push(1);
            q.push(2);
            q.push(3);
        } catch (const std::exception &e) {
            std::cerr << e.what() << '\n';
            REQUIRE(false);
        } catch (...) {
            REQUIRE(false);
        }
    }

    SUBCASE("test try_pop [FIFO]")
    {
        try {
            q.push(1);
            q.push(2);
            q.push(3);
            int pop_item;
            for (size_t i = 0; i < 3; ++i) {
                if (q.try_pop(pop_item)) {
                    REQUIRE(pop_item == arr[i]);
                } else {
                    std::cerr << "try_pop falied" << std::endl;
                    REQUIRE(false);
                }
            }
        } catch (const std::exception &e) {
            std::cerr << e.what() << '\n';
            REQUIRE(false);
        } catch (...) {
            REQUIRE(false);
        }
    }
}