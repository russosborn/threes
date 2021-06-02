cmake_minimum_required(VERSION 3.10)

# project name
project(threes)

#c++ requirements
set(CMAKE_CXX_STANDARD 14)
set(CMAKE_CXX_STANDARD_REQUIRED True)

add_library(GameSrc)

target_sources(GameSrc PUBLIC ${CMAKE_SOURCE_DIR}/game/src/Board.cc)
