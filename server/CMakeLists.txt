

file(GLOB_RECURSE server_file_lists ${CMAKE_CURRENT_SOURCE_DIR}/src/*.cpp)
add_executable(server ${server_file_lists} main.cpp)
target_include_directories(server PRIVATE ${CMAKE_CURRENT_SOURCE_DIR}/include)
target_link_libraries(server PRIVATE
# System libraries (Windows)
    Ws2_32

# Internal libraries
    liblogger   # lib/logger
    libqueue    # lib/queue

# Third-party libraries
    # nlohmann_json
    nlohmann_json::nlohmann_json
)