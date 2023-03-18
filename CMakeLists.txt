cmake_minimum_required(VERSION 3.10)
project(GameEngine)

set(CMAKE_CXX_STANDARD 14)

find_package(SDL2 REQUIRED)
include_directories(${SDL2_INCLUDE_DIRS})

add_executable(GameEngine
    ./srcs/main.cpp
    ./srcs/engine/Engine.cpp
    ./srcs/color/ColorBuffer.cpp
)

target_link_libraries(GameEngine ${SDL2_LIBRARIES})