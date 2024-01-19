# Require C++ 20
set(CMAKE_CXX_STANDARD 20)
set(CMAKE_CXX_STANDARD_REQUIRED ON)

# This grabs all files in the Engine directory that end with .cpp .h .hpp or .c
# and saves it in a variable called ${source_files}
file(GLOB source_files CONFIGURE_DEPENDS 
	"*.cpp" "*.h" "*.hpp" "*.c"
	"3dPrimitives/*.cpp" "3dPrimitives/*.h"
	"Components/*.cpp" "Components/*.h"
	"Graphics/*.cpp" "Graphics/*.h" "Graphics/*.c"
	"MemoryManager/*.cpp" "MemoryManager/*.h"
	"Entity/*.cpp" "Entity/*.h"
)
source_group(TREE ${CMAKE_CURRENT_SOURCE_DIR} FILES ${source_files})

# Create a library called engine that compiles the ${source_files}
add_library (engine ${source_files})

if(WIN32)
	# Link glfw library with engine
	target_link_libraries(engine glfw)
	# Link assimp library to engine
	target_link_libraries(engine assimp-vc142-mtd)
endif()
