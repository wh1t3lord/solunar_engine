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

if (USE_SHARED_LIBS)
	add_library(engine SHARED
		${ENGINE_SRC}
		${ENGINE_ENTITY_SRC}
		${ENGINE_PHYSICS_SRC}
	)
else ()
	add_library(engine STATIC
		${ENGINE_SRC}
		${ENGINE_ENTITY_SRC}
		${ENGINE_PHYSICS_SRC}
	)
endif()

if (USE_SHARED_LIBS)
	set_target_name_ex(engine "solunar_engine_dll")
	set_engine_out_dir(engine "${CMAKE_SOURCE_DIR}/bin")
endif()

target_precompile_headers(engine PRIVATE "enginepch.h")
target_include_directories(engine PRIVATE "${CMAKE_CURRENT_SOURCE_DIR}/../"
								  PRIVATE "${CMAKE_CURRENT_SOURCE_DIR}")

target_link_libraries(engine PUBLIC bullet core imgui)

if (USE_SHARED_LIBS)
	target_link_libraries(engine PRIVATE graphics)
	target_compile_definitions(engine PUBLIC ENGINE_DLL PRIVATE ENGINE_COMPILE PRIVATE GRAPHICS_COMPILE)
endif()

if (CMAKE_SIZEOF_VOID_P EQUAL 4)
	target_link_directories(engine PRIVATE "${CMAKE_SOURCE_DIR}/externals/assimp/libs/x32")
else()
	target_link_directories(engine PRIVATE "${CMAKE_SOURCE_DIR}/externals/assimp/libs/x64")
endif()