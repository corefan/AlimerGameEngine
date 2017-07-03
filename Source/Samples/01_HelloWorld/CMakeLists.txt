# Define target name
set (TARGET_NAME 01_HelloWorld)

# Define source files
file (GLOB H_FILES ${CMAKE_CURRENT_SOURCE_DIR}/*.h)
file (GLOB CPP_FILES ${CMAKE_CURRENT_SOURCE_DIR}/*.cpp)

set (HEADER_FILES ${H_FILES})
set (SOURCE_FILES ${CPP_FILES})
# set (CONFIG_FILES game.config)

include_directories(${ALIMER_SHADER_COMPILER_INCLUDE_DIR})

if (ALIMER_PLATFORM_ANDROID)
	include_directories(${ANDROID_NDK}/sources/android/native_app_glue)
endif (ALIMER_PLATFORM_ANDROID)

alimer_add_executable (${TARGET_NAME} ${HEADER_FILES} ${SOURCE_FILES} ${CONFIG_FILES})
target_link_libraries (${TARGET_NAME} AlimerShaderCompiler)

# Set Solution folder
set_target_properties (${TARGET_NAME} PROPERTIES FOLDER Samples)