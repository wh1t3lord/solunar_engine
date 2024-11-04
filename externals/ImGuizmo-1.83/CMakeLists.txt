set(IMGUIZMO_SRC_DIR "${CMAKE_CURRENT_SOURCE_DIR}")

file(GLOB IMGUIZMO_SRC
	"${IMGUIZMO_SRC_DIR}/*.cpp"
	"${IMGUIZMO_SRC_DIR}/*.h"
)

add_library(imguizmo STATIC 
	${IMGUIZMO_SRC}
)

target_include_directories(imguizmo PUBLIC "${IMGUIZMO_SRC_DIR}" "${CMAKE_CURRENT_SOURCE_DIR}/../imgui-1.90.1")
#target_link_libraries(imguizmo imgui)
