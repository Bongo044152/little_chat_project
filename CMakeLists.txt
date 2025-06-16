# main
cmake_minimum_required(VERSION 3.10)

project(
    "little chat project"
    VERSION 1.0
    DESCRIPTION "little chat project"
    LANGUAGES CXX
)

set(CMAKE_CXX_STANDARD 17)
set(CMAKE_C_STANDARD 17)

# build library
add_subdirectory(logger)

file(GLOB file_lists ${CMAKE_SOURCE_DIR}/src/*.cpp)
add_executable(main ${file_lists} main.cpp)
target_include_directories(main PUBLIC ./include)   # for directly include
target_include_directories(main PUBLIC .)           # for umbrella headers
target_link_libraries(main PRIVATE liblogger)

# for ctest
enable_testing()
add_subdirectory(test)