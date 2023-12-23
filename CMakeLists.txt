# Require CMake version 3.16
cmake_minimum_required (VERSION 3.16)

# Require C++ 20
set(CMAKE_CXX_STANDARD 20)
set(CMAKE_CXX_STANDARD_REQUIRED ON)

# Project name
project(GameEngine)

# Disable GLFW tests, examples, and documentation
set(GLFW_BUILD_DOCS OFF CACHE BOOL "" FORCE)
set(GLFW_BUILD_TESTS OFF CACHE BOOL "" FORCE)
set(GLFW_BUILD_EXAMPLES OFF CACHE BOOL "" FORCE)

# Include headers from include directory
include_directories(Libraries/GLAD/include)
include_directories(Libraries/GLM/include)
include_directories(Libraries/KHR/include)
include_directories(Libraries/Assimp/include)

# Include sub-projects.
add_subdirectory(Engine)
add_subdirectory(Game)
add_subdirectory(glfw)

# Link game with glfw
target_link_libraries(game glfw)