# Require C++ 20
set(CMAKE_CXX_STANDARD 20)
set(CMAKE_CXX_STANDARD_REQUIRED ON)

# This grabs all files in the Game directory that end with .cpp .h .hpp or .c
# and saves it in a variable called ${source_files}
file(GLOB source_files CONFIGURE_DEPENDS "*.cpp" "*.h" "*.hpp" "*.c")


### LINK ENGINE LIBRARY TO GAME PROJECT
# Include headers from Engine directory
include_directories(../Engine)

# Create an executable target called game and compiles the ${source_files}.
add_executable (game ${source_files} )

# Link game target with engine library
target_link_libraries(game engine)
