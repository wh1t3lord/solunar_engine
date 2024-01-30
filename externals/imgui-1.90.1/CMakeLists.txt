set(IMGUI_SRC_DIR "${CMAKE_CURRENT_SOURCE_DIR}")

file(GLOB IMGUI_SRC
	"${IMGUI_SRC_DIR}/*.cpp"
	"${IMGUI_SRC_DIR}/*.h"
)

add_library(imgui STATIC 
	${IMGUI_SRC}
)

if (WIN32)
	target_sources(imgui PRIVATE
		"${IMGUI_SRC_DIR}/backends/imgui_impl_win32.cpp"
	)
endif()

target_include_directories(imgui PUBLIC "${IMGUI_SRC_DIR}")
target_include_directories(imgui PUBLIC "${IMGUI_SRC_DIR}/backends")