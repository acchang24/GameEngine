# Require C++ 20
set(CMAKE_CXX_STANDARD 20)
set(CMAKE_CXX_STANDARD_REQUIRED ON)

# This grabs all files in the Engine directory that end with .cpp .h .hpp or .c
# and saves it in a variable called ${source_files}
file(GLOB source_files CONFIGURE_DEPENDS "*.cpp" "*.h" "*.hpp" "*.c")

# Create a library called engine that compiles the ${source_files}
add_library (engine ${source_files})
