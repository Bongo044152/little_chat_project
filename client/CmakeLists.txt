

file(GLOB_RECURSE client_file_lists ${CMAKE_CURRENT_SOURCE_DIR}/src/*.cpp)
add_executable(client ${client_file_lists} main.cpp)
target_include_directories(client PRIVATE ${CMAKE_CURRENT_SOURCE_DIR}/include)
target_link_libraries(client PRIVATE
# System libraries (Windows)
    Ws2_32
    Mswsock
    AdvApi32

# Internal libraries
    liblogger   # lib/logger
    libqueue    # lib/queue

# Third-party libraries
    # FTXUI
    ftxui::component
    ftxui::dom
    ftxui::screen

    # nlohmann_json
    nlohmann_json::nlohmann_json
)