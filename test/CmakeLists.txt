
# test logger
file(GLOB loggerlist ${CMAKE_CURRENT_SOURCE_DIR}/logger/*.cpp)
add_executable(test_logger ${loggerlist})
target_link_libraries(test_logger PRIVATE liblogger)
target_include_directories(test_logger PRIVATE ${CMAKE_CURRENT_SOURCE_DIR}) # for doctest
add_test(NAME logger_test COMMAND test_logger)

# test queue
file(GLOB queuelist ${CMAKE_CURRENT_SOURCE_DIR}/queue/*.cpp)
add_executable(test_queue ${queuelist})
target_link_libraries(test_queue PRIVATE libqueue)
target_include_directories(test_queue PRIVATE ${CMAKE_CURRENT_SOURCE_DIR}) # for doctest
add_test(NAME queue_test COMMAND test_queue)