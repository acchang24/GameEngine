﻿# Require C++ 20
set(CMAKE_CXX_STANDARD 20)
set(CMAKE_CXX_STANDARD_REQUIRED ON)

# This grabs all files in the Engine directory that end with .cpp .h .hpp or .c
# and saves it in a variable called ${source_files}
file(GLOB_RECURSE source_files CONFIGURE_DEPENDS 
	"Source/*.cpp" "Source/*.h" "Source/*.hpp" "Source/*.c"
)
source_group(TREE ${CMAKE_CURRENT_SOURCE_DIR} FILES ${source_files})

# Create a library called engine that compiles the ${source_files}
add_library (engine ${source_files})

if(WIN32)
	# Link assimp library to engine
	target_link_libraries(engine assimp-vc143-mt)
	# Link SDL library to engine
	target_link_libraries(engine SDL2 SDL2main SDL2_image SDL2_mixer)
	# Link FreeType library to engine
	target_link_libraries(engine freetype)
endif()
