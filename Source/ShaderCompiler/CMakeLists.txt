# Define target name
set (TARGET_NAME AlimerShaderCompiler)

# Define source files
file (GLOB H_FILES ${CMAKE_CURRENT_SOURCE_DIR}/include/*.h)
file (GLOB CPP_FILES ${CMAKE_CURRENT_SOURCE_DIR}/src/*.cpp)

# Add SPIRV-Cross files
FILE (GLOB SPIRV_CROSS_H_FILES ${ALIMER_EXTERNAL_DIR}/spirv-Cross/*.hpp)
FILE (GLOB SPIRV_CROSS_CPP_FILES ${ALIMER_EXTERNAL_DIR}/spirv-Cross/*.cpp)

set(HEADER_FILES ${H_FILES} ${SPIRV_CROSS_H_FILES})
set(SOURCE_FILES ${CPP_FILES} ${SPIRV_CROSS_CPP_FILES})

include_directories(
	${CMAKE_CURRENT_SOURCE_DIR}/include 
	${ALIMER_EXTERNAL_DIR}
	"${ALIMER_EXTERNAL_DIR}/glslang")

add_library (${TARGET_NAME} ${ALIMER_LIB_TYPE} ${HEADER_FILES} ${SOURCE_FILES})

if ( ALIMER_SHARED )
	target_compile_definitions(${TARGET_NAME} PRIVATE ALIMER_SHADER_COMPILER_EXPORTS=1)
endif ( ALIMER_SHARED )

if ( MSVC )
	target_compile_definitions(${TARGET_NAME} PRIVATE WIN32_LEAN_AND_MEAN NOMINMAX UNICODE _UNICODE)
endif ( MSVC )

target_link_libraries (${TARGET_NAME} glslang SPIRV)