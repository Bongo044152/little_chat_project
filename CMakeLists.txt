# main
cmake_minimum_required(VERSION 3.15)

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

# build client & server
add_subdirectory(client)
add_subdirectory(server)

# for ctest
enable_testing()
add_subdirectory(test)