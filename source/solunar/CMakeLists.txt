# Add core module
add_subdirectory(core)

# Add drivers

# Windows specific
if (MSVC)
	add_subdirectory(d3d11drv)
	add_subdirectory(d3d12drv)
	
	# FMOD currentrly only for Windows
	add_subdirectory(fmoddrv)
endif()

# Universal drivers
add_subdirectory(gldrv)
add_subdirectory(miniaudiodrv)

# Engine itself
add_subdirectory(engine)
add_subdirectory(graphics)
add_subdirectory(game)

# Shock Game
add_subdirectory(shockgame)

# Entry point
add_subdirectory(main)

##########
# Tools ##
##########

add_subdirectory(packer)