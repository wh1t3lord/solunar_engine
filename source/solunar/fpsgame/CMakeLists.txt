file(GLOB FPSGAME_SRC
	"${CMAKE_CURRENT_SOURCE_DIR}/*.h"
	"${CMAKE_CURRENT_SOURCE_DIR}/*.cpp"
)

source_group(TREE ${CMAKE_CURRENT_SOURCE_DIR} FILES ${FPSGAME_SRC})

if (USE_SHARED_LIBS)
	add_library(fpsgame SHARED
		${FPSGAME_SRC}
	)
else()
	add_library(fpsgame STATIC
		${FPSGAME_SRC}
	)
endif()

if (USE_SHARED_LIBS)
	set_target_name_ex(fpsgame "solunar_fpsgame_dll")
	set_engine_out_dir(fpsgame "${CMAKE_SOURCE_DIR}/bin")
endif()

target_precompile_headers(fpsgame PRIVATE "fpsgamepch.h")
target_include_directories(fpsgame PRIVATE "${CMAKE_CURRENT_SOURCE_DIR}/../"
								PRIVATE "${CMAKE_CURRENT_SOURCE_DIR}")
								
target_link_libraries(fpsgame engine graphics gameframework)

if (USE_SHARED_LIBS)
	target_compile_definitions(fpsgame PUBLIC FPSGAME_DLL PRIVATE FPSGAME_COMPILE)
endif()