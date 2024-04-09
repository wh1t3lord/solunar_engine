file(GLOB FPSGAME_SRC
	"${CMAKE_CURRENT_SOURCE_DIR}/*.h"
	"${CMAKE_CURRENT_SOURCE_DIR}/*.cpp"
)

source_group(TREE ${CMAKE_CURRENT_SOURCE_DIR} FILES ${FPSGAME_SRC})

add_library(fpsgame SHARED
	${FPSGAME_SRC}
)
	
target_precompile_headers(fpsgame PRIVATE "fpsgamepch.h")
target_include_directories(fpsgame PRIVATE "${CMAKE_CURRENT_SOURCE_DIR}/../"
								PRIVATE "${CMAKE_CURRENT_SOURCE_DIR}")
								
target_link_libraries(fpsgame engine graphics game)