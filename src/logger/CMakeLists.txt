add_library(logger STATIC logger.cpp)

target_include_directories(logger INTERFACE ${CMAKE_CURRENT_SOURCE_DIR})