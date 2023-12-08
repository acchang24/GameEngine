# Require CMake version 3.16
cmake_minimum_required (VERSION 3.16)

# Require C++ 20
set(CMAKE_CXX_STANDARD 20)
set(CMAKE_CXX_STANDARD_REQUIRED ON)

# Project name
project(GameEngine)

# Include sub-projects.
add_subdirectory("Engine")
add_subdirectory("Game")