option(SOLUNAR_ENABLE_FINAL "Enable final build cfg" ON)

if (MSVC)
    # Always generate PDBs
    add_compile_options(/Zi)
    add_link_options(/DEBUG)
	#add_compile_options(-fsanitize=address)
    add_compile_options(/W3) # Warning level
endif()

## Final
if (SOLUNAR_ENABLE_FINAL)
    SET(CMAKE_CXX_FLAGS_FINAL "${CMAKE_CXX_FLAGS}" CACHE STRING "Flags used by the C++ compiler during coverage builds." FORCE )
    SET(CMAKE_C_FLAGS_FINAL "${CMAKE_C_FLAGS}" CACHE STRING "Flags used by the C compiler during coverage builds." FORCE )
    SET(CMAKE_EXE_LINKER_FLAGS_FINAL "" CACHE STRING "Flags used for linking binaries during coverage builds." FORCE )
    SET(CMAKE_SHARED_LINKER_FLAGS_FINAL "" CACHE STRING "Flags used by the shared libraries linker during coverage builds." FORCE )
    MARK_AS_ADVANCED(CMAKE_CXX_FLAGS_FINAL CMAKE_C_FLAGS_FINAL CMAKE_EXE_LINKER_FLAGS_FINAL CMAKE_SHARED_LINKER_FLAGS_FINAL )
    
	add_compile_definitions("$<$<CONFIG:Final>:FINAL_BUILD>" )
	
    SET(SOLUNAR_CONFIGURATIONS_STR "Debug;RelWithDebInfo;Release;Final" CACHE STRING "" FORCE)
else()
    SET(SOLUNAR_CONFIGURATIONS_STR "Debug;RelWithDebInfo;Release" CACHE STRING "" FORCE)
endif()

# Wrap CMake general configs 
set(CMAKE_CONFIGURATION_TYPES ${SOLUNAR_CONFIGURATIONS_STR} CACHE STRING "" FORCE)
set(PREDEFINED_TARGETS_FOLDER "CustomTargets")

# Build config
if (SOLUNAR_ENABLE_FINAL)
    add_compile_options("$<$<CONFIG:Final>:/wd4530>" "$<$<CONFIG:Final>:/wd4251>" 
                        "$<$<CONFIG:Final>:/wd4275>" "$<$<CONFIG:Final>:/wd4577>" 
                        "$<$<CONFIG:Final>:/Ob2>")
                     #   "$<$<CONFIG:Final>:/Ob2>" "$<$<CONFIG:Final>:/WX>")
endif()

if (MSVC)
	if (MSVC_TOOLSET_VERSION EQUAL 143)
		set(PLATFORM_NAME "2022")
	elseif (MSVC_TOOLSET_VERSION EQUAL 142)
		set(PLATFORM_NAME "2019")
	elseif (MSVC_TOOLSET_VERSION EQUAL 141)
		set(PLATFORM_NAME "2017")
	elseif (MSVC_TOOLSET_VERSION EQUAL 140)
		set(PLATFORM_NAME "2015")
	endif()
endif()

function(set_target_name_engine TARGET NAME)
	if (CMAKE_SIZEOF_VOID_P EQUAL 4 AND WIN32)
		set(PLATFORM_STR "win32")
	elseif (WIN32)
		set(PLATFORM_STR "win64")
	endif()
	
	set_target_properties(${TARGET} PROPERTIES
		#OUTPUT_NAME "${NAME}_${PLATFORM_STR}_${PLATFORM_NAME}"
		OUTPUT_NAME "${NAME}_${PLATFORM_STR}"
	
		DEBUG_POSTFIX "_d"
		RELEASE_POSTFIX "_r"
		RELWITHDEBINFO_POSTFIX "_rwdi"
		MINSIZEREL_POSTFIX "_msr"
		FINAL_POSTFIX "_f"
	)
endfunction()

function(set_target_name TARGET)
	set_target_properties(${TARGET} PROPERTIES
		OUTPUT_NAME "${TARGET}"
		
		DEBUG_POSTFIX "_d"
		RELEASE_POSTFIX "_r"
		RELWITHDEBINFO_POSTFIX "_rwdi"
		MINSIZEREL_POSTFIX "_msr"
		FINAL_POSTFIX "_f"
	)
endfunction()

function(set_target_name_ex TARGET NAME)
	set_target_properties(${TARGET} PROPERTIES
		OUTPUT_NAME "${NAME}"
		
		DEBUG_POSTFIX "_d"
		RELEASE_POSTFIX "_r"
		RELWITHDEBINFO_POSTFIX "_rwdi"
		MINSIZEREL_POSTFIX "_msr"
		FINAL_POSTFIX "_f"
	)
endfunction()

function(set_target_out_dir TARGET PATH)
	set_target_properties(${TARGET} PROPERTIES
		RUNTIME_OUTPUT_DIRECTORY_DEBUG "${PATH}"
		RUNTIME_OUTPUT_DIRECTORY_RELEASE "${PATH}"
		RUNTIME_OUTPUT_DIRECTORY_RELWITHDEBINFO "${PATH}"
		RUNTIME_OUTPUT_DIRECTORY_MINSIZEREL "${PATH}"
		RUNTIME_OUTPUT_DIRECTORY_FINAL "${PATH}"
	)
endfunction()

function(set_engine_out_dir TARGET PATH)
	if (CMAKE_SIZEOF_VOID_P EQUAL 4 AND WIN32)
		set(PLATFORM_STR "win32")
	elseif (WIN32)
		set(PLATFORM_STR "win64")
	endif()

	set_target_properties(${TARGET} PROPERTIES
		RUNTIME_OUTPUT_DIRECTORY_DEBUG 				"${PATH}/${PLATFORM_STR}"
		RUNTIME_OUTPUT_DIRECTORY_RELEASE 			"${PATH}/${PLATFORM_STR}"
		RUNTIME_OUTPUT_DIRECTORY_RELWITHDEBINFO		"${PATH}/${PLATFORM_STR}"
		RUNTIME_OUTPUT_DIRECTORY_MINSIZEREL 		"${PATH}/${PLATFORM_STR}"
		RUNTIME_OUTPUT_DIRECTORY_FINAL 				"${PATH}/${PLATFORM_STR}"
	)
endfunction()


# Parse the directory and get file list
macro(add_source_directory DIRNAME SRC_PATH SOURCE_GROUP_PATH)

	# lets find files
	file(GLOB DIRNAME
		"${CMAKE_CURRENT_SOURCE_DIR}/${SRC_PATH}/*.h"
		"${CMAKE_CURRENT_SOURCE_DIR}/${SRC_PATH}/*.cpp"
	)
	
	# Add source group to MSVC
	source_group("${SOURCE_GROUP_PATH}" FILES ${DIRNAME})
	
endmacro()