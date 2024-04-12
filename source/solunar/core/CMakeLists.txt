file(GLOB CORE_SRC
	"${CMAKE_CURRENT_SOURCE_DIR}/*.cpp"
	"${CMAKE_CURRENT_SOURCE_DIR}/*.h"
)

file(GLOB CORE_PLATFORM_SRC
	"${CMAKE_CURRENT_SOURCE_DIR}/platform/*.cpp"
	"${CMAKE_CURRENT_SOURCE_DIR}/platform/*.h"
)

file(GLOB CORE_MEMORY_SRC
	"${CMAKE_CURRENT_SOURCE_DIR}/memory/*.cpp"
	"${CMAKE_CURRENT_SOURCE_DIR}/memory/*.h"
)

file(GLOB CORE_UTILS_SRC
	"${CMAKE_CURRENT_SOURCE_DIR}/utils/*.cpp"
	"${CMAKE_CURRENT_SOURCE_DIR}/utils/*.h"
)

file(GLOB CORE_MATH_SRC
	"${CMAKE_CURRENT_SOURCE_DIR}/math/*.cpp"
	"${CMAKE_CURRENT_SOURCE_DIR}/math/*.h"
)

file(GLOB CORE_OBJECT_SRC
	"${CMAKE_CURRENT_SOURCE_DIR}/object/*.cpp"
	"${CMAKE_CURRENT_SOURCE_DIR}/object/*.h"
)

file(GLOB CORE_FILE_SRC
	"${CMAKE_CURRENT_SOURCE_DIR}/file/*.h"
	"${CMAKE_CURRENT_SOURCE_DIR}/file/*.cpp"
)

source_group(TREE ${CMAKE_CURRENT_SOURCE_DIR} FILES ${CORE_SRC})
source_group(TREE ${CMAKE_CURRENT_SOURCE_DIR} FILES ${CORE_PLATFORM_SRC})
source_group(TREE ${CMAKE_CURRENT_SOURCE_DIR} FILES ${CORE_MEMORY_SRC})
source_group(TREE ${CMAKE_CURRENT_SOURCE_DIR} FILES ${CORE_UTILS_SRC})
source_group(TREE ${CMAKE_CURRENT_SOURCE_DIR} FILES ${CORE_MATH_SRC})
source_group(TREE ${CMAKE_CURRENT_SOURCE_DIR} FILES ${CORE_OBJECT_SRC})
source_group(TREE ${CMAKE_CURRENT_SOURCE_DIR} FILES ${CORE_FILE_SRC})

if (USE_SHARED_LIBS)
	add_library(core SHARED
		${CORE_SRC}
		${CORE_PLATFORM_SRC}
		${CORE_MEMORY_SRC}
		${CORE_UTILS_SRC}
		${CORE_MATH_SRC}
		${CORE_OBJECT_SRC}
		${CORE_FILE_SRC}
	)
else()
	add_library(core STATIC
		${CORE_SRC}
		${CORE_PLATFORM_SRC}
		${CORE_MEMORY_SRC}
		${CORE_UTILS_SRC}
		${CORE_MATH_SRC}
		${CORE_OBJECT_SRC}
		${CORE_FILE_SRC}
	)
endif()

if (USE_SHARED_LIBS)
	set_target_name_ex(core "solunar_core_dll")
	set_engine_out_dir(core "${CMAKE_SOURCE_DIR}/bin")
endif()

target_include_directories(core PRIVATE "${CMAKE_CURRENT_SOURCE_DIR}/../")
target_include_directories(core PUBLIC "${CMAKE_SOURCE_DIR}/externals/glm")
target_include_directories(core PUBLIC "${CMAKE_SOURCE_DIR}/externals/stb")

target_link_libraries(core tinyxml2 lz4)

if (USE_SHARED_LIBS)
	target_compile_definitions(core PUBLIC CORE_DLL PRIVATE CORE_COMPILE)
endif()