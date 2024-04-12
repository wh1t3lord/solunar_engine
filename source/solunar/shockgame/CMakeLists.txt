file(GLOB SHOCKGAME_SRC
	"${CMAKE_CURRENT_SOURCE_DIR}/*.h"
	"${CMAKE_CURRENT_SOURCE_DIR}/*.cpp"
)

source_group(TREE ${CMAKE_CURRENT_SOURCE_DIR} FILES ${SHOCKGAME_SRC})

if (USE_SHARED_LIBS)
	add_library(shockgame SHARED
	${SHOCKGAME_SRC}
	)
else()
	add_library(shockgame STATIC
		${SHOCKGAME_SRC}
	)
endif()

if (USE_SHARED_LIBS)
	set_target_name_ex(shockgame "solunar_shockgame_dll")
	set_engine_out_dir(shockgame "${CMAKE_SOURCE_DIR}/bin")
endif()
	
target_precompile_headers(shockgame PRIVATE "shockgamepch.h")
target_include_directories(shockgame PRIVATE "${CMAKE_CURRENT_SOURCE_DIR}/../"
								PRIVATE "${CMAKE_CURRENT_SOURCE_DIR}")
								
target_link_libraries(shockgame engine game)

if (USE_SHARED_LIBS)
	target_compile_definitions(shockgame PUBLIC SHOCKGAME_DLL PRIVATE SHOCKGAME_COMPILE)
endif()