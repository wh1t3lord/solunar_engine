file(GLOB ENGINE_SRC
	"${CMAKE_CURRENT_SOURCE_DIR}/*.cpp"
	"${CMAKE_CURRENT_SOURCE_DIR}/*.h"
)

file(GLOB ENGINE_ENTITY_SRC
	"${CMAKE_CURRENT_SOURCE_DIR}/entity/*.cpp"
	"${CMAKE_CURRENT_SOURCE_DIR}/entity/*.h"
)

file(GLOB ENGINE_PHYSICS_SRC
	"${CMAKE_CURRENT_SOURCE_DIR}/physics/*.cpp"
	"${CMAKE_CURRENT_SOURCE_DIR}/physics/*.h"
)

source_group(TREE ${CMAKE_CURRENT_SOURCE_DIR} FILES ${ENGINE_SRC})
source_group(TREE ${CMAKE_CURRENT_SOURCE_DIR} FILES ${ENGINE_ENTITY_SRC})
source_group(TREE ${CMAKE_CURRENT_SOURCE_DIR} FILES ${ENGINE_PHYSICS_SRC})

add_library(engine STATIC
	${ENGINE_SRC}
	${ENGINE_ENTITY_SRC}
	${ENGINE_PHYSICS_SRC}
)

target_precompile_headers(engine PRIVATE "enginepch.h")
target_include_directories(engine PRIVATE "${CMAKE_CURRENT_SOURCE_DIR}/../"
								  PRIVATE "${CMAKE_CURRENT_SOURCE_DIR}")

target_link_libraries(engine bullet graphics core)