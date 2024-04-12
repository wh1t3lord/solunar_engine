set(IMGUI_SRC_DIR "${CMAKE_CURRENT_SOURCE_DIR}")

file(GLOB IMGUI_SRC
	"${IMGUI_SRC_DIR}/*.cpp"
	"${IMGUI_SRC_DIR}/*.h"
)

if (USE_SHARED_LIBS)
    add_library(imgui SHARED 
    	${IMGUI_SRC}
    )
else ()
    add_library(imgui SHARED 
    	${IMGUI_SRC}
    )
endif()

if (WIN32)
	target_sources(imgui PRIVATE
		"${IMGUI_SRC_DIR}/backends/imgui_impl_win32.cpp"
	)
endif()

target_include_directories(imgui PUBLIC "${IMGUI_SRC_DIR}")
target_include_directories(imgui PUBLIC "${IMGUI_SRC_DIR}/backends")

if (USE_SHARED_LIBS)
	set_target_name_ex(imgui "imgui_dll")
	set_engine_out_dir(imgui "${CMAKE_SOURCE_DIR}/bin")
	target_compile_definitions(imgui PUBLIC IMGUI_DLL PRIVATE IMGUI_COMPILE)
endif()
