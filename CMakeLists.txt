cmake_minimum_required(VERSION 3.28)
project(BouncingBall VERSION 1.0)

set(CMAKE_CXX_STANDARD 11)
set(CMAKE_CXX_STANDARD_REQUIRED True)

find_package(SDL2 REQUIRED)
include_directories("${SDL2_INCLUDE_DIRS}")

add_executable(bouncing-game main.cpp game.cpp)

target_link_libraries(bouncing-game ${SDL2_LIBRARIES})
