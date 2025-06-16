# logger/CMakeLists.txt
# for buding logger lib

file(GLOB LOGGER_SOURCES ${CMAKE_CURRENT_SOURCE_DIR}/src/*.cpp)

add_library(logger STATIC ${LOGGER_SOURCES})
target_include_directories(logger PUBLIC ${CMAKE_CURRENT_SOURCE_DIR}/include )
