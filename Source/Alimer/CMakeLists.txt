# Define target name
set (TARGET_NAME Alimer)

alimer_generate_library_version_variables(${ALIMER_VERSION_MAJOR} ${ALIMER_VERSION_MINOR} ${ALIMER_VERSION_PATCH})
configure_file (${CMAKE_CURRENT_SOURCE_DIR}/include/Version.in.h ${CMAKE_CURRENT_SOURCE_DIR}/include/Version.h)

# Define source files
file (GLOB H_FILES ${CMAKE_CURRENT_SOURCE_DIR}/include/*.h)
file (GLOB CPP_FILES ${CMAKE_CURRENT_SOURCE_DIR}/src/*.cpp)

# Define SPIRV-Cross files
FILE (GLOB SPIRV_CROSS_H_FILES ${ALIMER_EXTERNAL_DIR}/spirv-Cross/*.hpp)
FILE (GLOB SPIRV_CROSS_CPP_FILES ${ALIMER_EXTERNAL_DIR}/spirv-Cross/*.cpp)

set(HEADER_FILES ${H_FILES} ${SPIRV_CROSS_H_FILES})
set(SOURCE_FILES ${CPP_FILES} ${SPIRV_CROSS_CPP_FILES})

include_directories(${CMAKE_CURRENT_SOURCE_DIR}/src)
include_directories(${ALIMER_EXTERNAL_DIR})

# Add engine sub folders, both include and src
alimer_add_directory(Core)
alimer_add_directory(Utils)
alimer_add_directory(IO)
alimer_add_directory(Math)
alimer_add_directory(Engine)
alimer_add_directory(Graphics)
alimer_add_directory(Audio)

if(ALIMER_PLATFORM_WINDOWS OR ALIMER_PLATFORM_UWP)
	alimer_add_platform_directory(Core Windows)
	alimer_add_platform_directory(Engine Windows)
	alimer_add_platform_directory(Graphics Direct3D12)
	alimer_add_platform_directory(Audio XAudio2)
elseif(ALIMER_PLATFORM_ANDROID)
	alimer_add_platform_directory(Core Android)
	alimer_add_platform_directory(Engine Android)
elseif(ALIMER_PLATFORM_WEB)
	alimer_add_platform_directory(Core HTML5)
	alimer_add_platform_directory(Engine HTML5)
endif()

add_library (${TARGET_NAME} ${ALIMER_LIB_TYPE} ${HEADER_FILES} ${SOURCE_FILES})

if ( ALIMER_SHARED )
	target_compile_definitions(${TARGET_NAME} PRIVATE ALIMER_EXPORTS=1)
endif ( ALIMER_SHARED )

if ( MSVC )
	target_compile_definitions(${TARGET_NAME} PRIVATE STRICT WIN32_LEAN_AND_MEAN NOMINMAX UNICODE _UNICODE)
endif ( MSVC )

if(ALIMER_PLATFORM_WINDOWS)
	find_library(D3DCOMPILER_LIBRARY NAMES d3dcompiler.lib HINTS ${WIN10_SDK_LIB_PATH})
	target_link_libraries(${TARGET_NAME} PRIVATE Winmm ws2_32 iphlpapi)
	target_link_libraries(${TARGET_NAME} PRIVATE ${D3DCOMPILER_LIBRARY})
elseif (ALIMER_PLATFORM_ANDROID)
	target_link_libraries(${TARGET_NAME} android log)
endif()