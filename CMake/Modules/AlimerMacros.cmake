cmake_minimum_required(VERSION 3.8)

# use include guard.
if( _CMAKE_ALIMER_MACROS_GUARD_ )
	return()
endif()
set(_CMAKE_ALIMER_MACROS_GUARD_ 1)

macro(alimer_generate_library_version_variables MAJOR MINOR PATCH)
  set(LIBRARY_VERSION_MAJOR ${MAJOR})
  set(LIBRARY_VERSION_MINOR ${MINOR})
  set(LIBRARY_VERSION_PATCH ${PATCH})
  set(LIBRARY_VERSION ${LIBRARY_VERSION_MAJOR}.${LIBRARY_VERSION_MINOR}.${LIBRARY_VERSION_PATCH})
  set(LIBRARY_VERSION_STR "${LIBRARY_VERSION_MAJOR}.${LIBRARY_VERSION_MINOR}.${LIBRARY_VERSION_PATCH}")
  math(EXPR LIBRARY_VERSION_CALC "${LIBRARY_VERSION_MAJOR}*1000 + ${LIBRARY_VERSION_MINOR}*100 + ${LIBRARY_VERSION_PATCH}")
endmacro(alimer_generate_library_version_variables)

# Macro to add files from a directory grouped to the solution file
macro (alimer_add_directory DIRECTORY)
    file (GLOB DIR_HEADER_FILES include/${DIRECTORY}/*.h include/${DIRECTORY}/*.hpp include/${DIRECTORY}/*.inl )
    file (GLOB DIR_SOURCE_FILES src/${DIRECTORY}/*.c src/${DIRECTORY}/*.cpp src/${DIRECTORY}/*.h src/${DIRECTORY}/*.hpp)
    
    get_filename_component (GROUP ${DIRECTORY} NAME)
    if(MSVC)
    	source_group("Header Files\\${GROUP}" FILES ${DIR_HEADER_FILES})
	source_group("Source Files\\${GROUP}" FILES ${DIR_SOURCE_FILES})
    else()
    	source_group ("${GROUP}" FILES ${DIR_SOURCE_FILES})
    endif()
    list (APPEND SOURCE_FILES ${DIR_HEADER_FILES} ${DIR_SOURCE_FILES})
endmacro ()

macro (alimer_add_platform_directory BASE_DIR SUB_DIRECTORY)
    file (GLOB DIR_HEADER_FILES include/${BASE_DIR}/${SUB_DIRECTORY}/*.h include/${BASE_DIR}/${SUB_DIRECTORY}/*.hpp include/${BASE_DIR}/${SUB_DIRECTORY}/*.inl)
    file (GLOB DIR_SOURCE_FILES src/${BASE_DIR}/${SUB_DIRECTORY}/*.c src/${BASE_DIR}/${SUB_DIRECTORY}/*.cpp src/${BASE_DIR}/${SUB_DIRECTORY}/*.h src/${BASE_DIR}/${SUB_DIRECTORY}/*.h)
    
    get_filename_component (GROUP ${SUB_DIRECTORY} NAME)
    if(MSVC)
    	source_group("Header Files\\${BASE_DIR}\\${GROUP}" FILES ${DIR_HEADER_FILES})
	source_group("Source Files\\${BASE_DIR}\\${GROUP}" FILES ${DIR_SOURCE_FILES})
    else()
    	source_group ("${GROUP}" FILES ${DIR_SOURCE_FILES})
    endif()
    
    list (APPEND SOURCE_FILES ${DIR_HEADER_FILES} ${DIR_SOURCE_FILES})
endmacro ()

FUNCTION(alimer_add_sample TARGET_NAME)	
	if (ALIMER_PLATFORM_UWP)
		# alimer_uwp_add_target_properties(${TARGET_NAME})
	endif (ALIMER_PLATFORM_UWP)
	
	if(ALIMER_PLATFORM_WINDOWS OR ALIMER_PLATFORM_UWP)
		add_executable (${TARGET_NAME} WIN32 ${ARGN} ${EXTRA_FILES})
	else()
		add_library (${TARGET_NAME} SHARED ${ARGN} ${EXTRA_FILES})
	endif()
	
	target_link_libraries (${TARGET_NAME} Alimer)
ENDFUNCTION(alimer_add_sample)