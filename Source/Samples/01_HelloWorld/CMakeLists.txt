# Define target name
set (TARGET_NAME 01_HelloWorld)

# Define source files
file (GLOB H_FILES ${CMAKE_CURRENT_SOURCE_DIR}/*.h)
file (GLOB CPP_FILES ${CMAKE_CURRENT_SOURCE_DIR}/*.cpp)

set (HEADER_FILES ${H_FILES})
set (SOURCE_FILES ${CPP_FILES})
# set (CONFIG_FILES game.config)

alimer_add_executable (${TARGET_NAME} ${HEADER_FILES} ${SOURCE_FILES} ${CONFIG_FILES})

# Set Solution folder
set_target_properties (${TARGET_NAME} PROPERTIES FOLDER Samples)