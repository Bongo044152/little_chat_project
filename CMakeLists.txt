cmake_minimum_required(VERSION 3.15)

# ──────────────────────────────────────────────────────────────
#  Project definition
# ──────────────────────────────────────────────────────────────
project(
    Chat_Project
    VERSION 1.0
    DESCRIPTION "Little chat project with client/server architecture"
    LANGUAGES CXX
)

# ──────────────────────────────────────────────────────────────
#  Compiler settings
# ──────────────────────────────────────────────────────────────
set(CMAKE_CXX_STANDARD 17)
set(CMAKE_CXX_STANDARD_REQUIRED ON)
set(CMAKE_C_STANDARD 17)

# ──────────────────────────────────────────────────────────────
#  Options
# ──────────────────────────────────────────────────────────────

option(ENABLE_TEST "Enable building unit tests" OFF)
set(LOG_LEVEL 0 CACHE STRING "Logging level (4=Critical, 3=ERROR, 2=WARN, 1=INFO, 0=DEBUG)")

# ──────────────────────────────────────────────────────────────
#  Display options summary (like FTXUI style)
# ──────────────────────────────────────────────────────────────

function(options_message msg)
    message(STATUS "${msg}")
endfunction()

options_message("┌─ Project Options ──────────────────────────────")
options_message("│ ENABLE_TEST      : ${ENABLE_TEST}")
options_message("│ LOG_LEVEL        : ${LOG_LEVEL}")
options_message("└───────────────────────────────────────────────")

# ──────────────────────────────────────────────────────────────
#  Compile definitions
# ──────────────────────────────────────────────────────────────

add_compile_definitions(LOG_LEVEL=${LOG_LEVEL})

if(ENABLE_TEST)
    add_compile_definitions(ENABLE_TEST)
endif()

# ──────────────────────────────────────────────────────────────
#  Build targets
# ──────────────────────────────────────────────────────────────

add_subdirectory(lib)
add_subdirectory(client)
add_subdirectory(server)

# ──────────────────────────────────────────────────────────────
#  Enable tests if requested
# ──────────────────────────────────────────────────────────────

if(ENABLE_TEST)
    enable_testing()
    add_subdirectory(test)
endif()