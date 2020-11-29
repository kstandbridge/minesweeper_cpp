add_library(game STATIC game.cpp)

target_include_directories(game INTERFACE ${CMAKE_CURRENT_SOURCE_DIR})