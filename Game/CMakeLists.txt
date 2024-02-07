# Require C++ 20
set(CMAKE_CXX_STANDARD 20)
set(CMAKE_CXX_STANDARD_REQUIRED ON)

# This grabs all files in the Game directory that end with .cpp .h .hpp or .c
# and saves it in a variable called ${source_files}
file(GLOB_RECURSE source_files CONFIGURE_DEPENDS "Source/*.cpp" "Source/*.h" "Source/*.hpp" "Source/*.c")
source_group(TREE "${CMAKE_CURRENT_SOURCE_DIR}" FILES ${source_files})

### LINK ENGINE LIBRARY TO GAME PROJECT
# Include headers from Engine directory
include_directories(../Engine/Source)

# Create an executable target called game and compiles the ${source_files}.
add_executable (game ${source_files} )

if(WIN32)
	# Link game target with engine library
	target_link_libraries(game engine)

	# Copy dlls to build
	file(GLOB_RECURSE MYDLLS "${PROJECT_SOURCE_DIR}/Libraries/*.dll")
	foreach(CurrentDllFile IN LISTS MYDLLS)
		add_custom_command(TARGET game
			POST_BUILD
			COMMAND ${CMAKE_COMMAND} -E copy "${CurrentDllFile}" "${CMAKE_CURRENT_BINARY_DIR}"
			COMMENT "Copy dll file to ${CMAKE_CURRENT_BINARY_DIR} directory" VERBATIM
		)
	endforeach()

	#set_target_properties(game PROPERTIES LINK_FLAGS "/ENTRY:mainCRTStartup /SUBSYSTEM:WINDOWS")
endif()
