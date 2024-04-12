file(GLOB GRAPHICS_SRC
	"${CMAKE_CURRENT_SOURCE_DIR}/*.h"
	"${CMAKE_CURRENT_SOURCE_DIR}/*.cpp"
)

file(GLOB GRAPHICS_CORE_SRC
	"${CMAKE_CURRENT_SOURCE_DIR}/core/*.h"
	"${CMAKE_CURRENT_SOURCE_DIR}/core/*.cpp"
)

file(GLOB GRAPHICS_MATERIALS_SRC
	"${CMAKE_CURRENT_SOURCE_DIR}/materials/*.h"
	"${CMAKE_CURRENT_SOURCE_DIR}/materials/*.cpp"
)

source_group(TREE ${CMAKE_CURRENT_SOURCE_DIR} FILES ${GRAPHICS_SRC})
source_group(TREE ${CMAKE_CURRENT_SOURCE_DIR} FILES ${GRAPHICS_CORE_SRC})
source_group(TREE ${CMAKE_CURRENT_SOURCE_DIR} FILES ${GRAPHICS_MATERIALS_SRC})

add_library(graphics STATIC
	${GRAPHICS_SRC}
	${GRAPHICS_CORE_SRC}
	${GRAPHICS_MATERIALS_SRC}
)

target_precompile_headers(graphics PRIVATE "graphicspch.h")
target_include_directories(graphics PRIVATE "${CMAKE_CURRENT_SOURCE_DIR}/../"
				    PRIVATE "${CMAKE_CURRENT_SOURCE_DIR}")

target_include_directories(graphics PRIVATE "${CMAKE_SOURCE_DIR}/externals/assimp/include")

target_link_libraries(graphics imgui core)

if (USE_SHARED_LIBS)
	# TODO: Move D3D11Drv to DLL
	target_link_libraries(graphics d3d11drv)
	
	#target_compile_definitions(graphics PUBLIC ENGINE_DLL PRIVATE GRAPHICS_COMPILE)
endif()