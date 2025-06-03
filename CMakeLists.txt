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

# Include headers from libraries
include_directories(Libraries/GLAD/include)
include_directories(Libraries/GLM/include)
include_directories(Libraries/KHR/include)
include_directories(Libraries/Assimp/include)
include_directories(Libraries/SDL/include)

if(WIN32)
	link_directories(Libraries/Assimp/lib/win)
	link_directories(Libraries/SDL/lib/win)
endif()

# Subdirectories to build
add_subdirectory(Engine)
add_subdirectory(Game)
