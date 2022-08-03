cmake_minimum_required(VERSION 3.23)
project(tetris)

set(CMAKE_CXX_STANDARD 11)

add_executable(tetris main.cpp display.cpp display.h)

target_link_libraries(tetris SDL2main SDL2 SDL2_ttf)
