# Define target name
set (TARGET_NAME Alimer)

alimer_generate_library_version_variables(${ALIMER_VERSION_MAJOR} ${ALIMER_VERSION_MINOR} ${ALIMER_VERSION_PATCH})
configure_file (${CMAKE_CURRENT_SOURCE_DIR}/include/Version.in.h ${CMAKE_CURRENT_SOURCE_DIR}/include/Version.h)

# Define source files
file (GLOB H_FILES ${CMAKE_CURRENT_SOURCE_DIR}/include/*.h)
file (GLOB CPP_FILES ${CMAKE_CURRENT_SOURCE_DIR}/src/*.cpp)
set(HEADER_FILES ${H_FILES})
set(SOURCE_FILES ${CPP_FILES})

# Add engine sub folders, both include and src
alimer_add_directory(Core)

add_library (${TARGET_NAME} ${ALIMER_LIB_TYPE} ${HEADER_FILES} ${SOURCE_FILES})

if ( ALIMER_SHARED )
	target_compile_definitions(${TARGET_NAME} PRIVATE ALIMER_EXPORTS=1)
endif ( ALIMER_SHARED )

if ( MSVC )
	target_compile_definitions(${TARGET_NAME} PRIVATE STRICT WIN32_LEAN_AND_MEAN NOMINMAX)
endif ( MSVC )

if(ALIMER_PLATFORM_WINDOWS)
	target_link_libraries(${TARGET_NAME} PRIVATE Winmm ws2_32 iphlpapi)
endif()