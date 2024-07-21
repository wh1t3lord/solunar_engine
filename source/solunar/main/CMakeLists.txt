file(GLOB MAIN_SRC
	"${CMAKE_CURRENT_SOURCE_DIR}/*.cpp"
	"${CMAKE_CURRENT_SOURCE_DIR}/*.h"
)

source_group(TREE ${CMAKE_CURRENT_SOURCE_DIR} FILES ${MAIN_SRC})

add_executable(main WIN32 ${MAIN_SRC})

set_target_name_ex(main "solunar_main")
set_engine_out_dir(main "${CMAKE_SOURCE_DIR}/bin")

target_include_directories(main PRIVATE "${CMAKE_CURRENT_SOURCE_DIR}/../"
								  PRIVATE "${CMAKE_CURRENT_SOURCE_DIR}")

target_link_libraries(main shockgame fmoddrv d3d11drv engine core)

if (CMAKE_SIZEOF_VOID_P EQUAL 4)
	target_link_directories(main PRIVATE "${CMAKE_SOURCE_DIR}/externals/assimp/libs/x32")
	target_link_directories(main PRIVATE "${CMAKE_SOURCE_DIR}/externals/fmod_studio/lib/x86")
	target_link_directories(main PRIVATE "${CMAKE_SOURCE_DIR}/externals/fmod/lib/x86")
else()
	target_link_directories(main PRIVATE "${CMAKE_SOURCE_DIR}/externals/assimp/libs/x64")
	target_link_directories(main PRIVATE "${CMAKE_SOURCE_DIR}/externals/fmod_studio/lib/x64")
	target_link_directories(main PRIVATE "${CMAKE_SOURCE_DIR}/externals/fmod/lib/x64")

	add_custom_command(TARGET main POST_BUILD
		COMMAND ${CMAKE_COMMAND} -E copy_if_different
		"${CMAKE_SOURCE_DIR}/externals/fmod_studio/lib/x64/fmodstudio.dll"
		$<TARGET_FILE_DIR:main>/fmodstudio.dll)

	add_custom_command(TARGET main POST_BUILD
		COMMAND ${CMAKE_COMMAND} -E copy_if_different
		"${CMAKE_SOURCE_DIR}/externals/fmod_studio/lib/x64/fmodstudioL.dll"
		$<TARGET_FILE_DIR:main>/fmodstudioL.dll)

	add_custom_command(TARGET main POST_BUILD
		COMMAND ${CMAKE_COMMAND} -E copy_if_different
		"${CMAKE_SOURCE_DIR}/externals/fmod/lib/x64/fmod.dll"
		$<TARGET_FILE_DIR:main>/fmod.dll)

	add_custom_command(TARGET main POST_BUILD
		COMMAND ${CMAKE_COMMAND} -E copy_if_different
		"${CMAKE_SOURCE_DIR}/externals/fmod/lib/x64/fmodL.dll"
		$<TARGET_FILE_DIR:main>/fmodL.dll)
endif()
