# logger/CMakeLists.txt
# for buding logger lib

file(GLOB LOGGER_SOURCES ${CMAKE_CURRENT_SOURCE_DIR}/src/*.cpp)

add_library(liblogger STATIC ${LOGGER_SOURCES})
target_include_directories(liblogger PUBLIC ${CMAKE_CURRENT_SOURCE_DIR}/include)