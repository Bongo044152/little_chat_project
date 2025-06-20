# main
cmake_minimum_required(VERSION 3.10)

project(
    "Chat_Project"
    VERSION 1.0
    DESCRIPTION "little chat project"
    LANGUAGES CXX
)

set(CMAKE_CXX_STANDARD 17)
set(CMAKE_CXX_STANDARD_REQUIRED on)
set(CMAKE_C_STANDARD 17)

# build library
add_subdirectory(lib)

# build external library
include(cmake/FTXUI.cmake)
include(cmake/Json.cmake)

file(GLOB_RECURSE file_lists ${CMAKE_SOURCE_DIR}/src/*.cpp)
add_executable(main ${file_lists} main.cpp)
target_include_directories(main PRIVATE ./include)
target_link_libraries(main PRIVATE
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

# for ctest
enable_testing()
add_subdirectory(test)