file (GLOB_RECURSE BULLET_COLLISION_SRC
	"BulletCollision/*.cpp"
	"BulletCollision/*.h")

file (GLOB_RECURSE BULLET_DYNAMICS_SRC
	"BulletDynamics/*.cpp"
	"BulletDynamics/*.h")

file (GLOB_RECURSE BULLET_LinearMath_SRC
	"LinearMath/*.cpp"
	"LinearMath/*.h")

add_library(bullet STATIC 
	${BULLET_LinearMath_SRC}
	${BULLET_COLLISION_SRC}
	${BULLET_DYNAMICS_SRC})

target_include_directories(bullet PUBLIC "${CMAKE_CURRENT_SOURCE_DIR}")