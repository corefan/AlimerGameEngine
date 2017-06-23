cmake_minimum_required(VERSION 3.8)
if (COMMAND cmake_policy)
    # Libraries linked via full path no longer produce linker search paths
    cmake_policy (SET CMP0003 NEW)
    # INTERFACE_LINK_LIBRARIES defines the link interface
    cmake_policy (SET CMP0022 NEW)
    # Disallow use of the LOCATION target property - so we set to OLD as we still need it
    cmake_policy (SET CMP0026 OLD)
    # MACOSX_RPATH is enabled by default
    cmake_policy (SET CMP0042 NEW)
endif ()

if(MSVC)
	CMAKE_VS_WINDOWS_TARGET_PLATFORM_VERSION("10.0.10586.0")
endif()

# Setup CMake project
project (Alimer)

# Get paths and store as variables
get_filename_component(ALIMER_ROOT_DIR "." ABSOLUTE)
set (ALIMER_EXTERNAL_DIR "${ALIMER_ROOT_DIR}/external")
set (ALIMER_INCLUDE_DIR "${ALIMER_ROOT_DIR}/Source/Alimer/include")

# Set CMake modules search path
set(CMAKE_MODULE_PATH ${CMAKE_MODULE_PATH} ${CMAKE_SOURCE_DIR}/CMake/Modules)
# load required Alimer cmake macros
include(Alimer REQUIRED)

# Set paths
if (ALIMER_PLATFORM_ANDROID)
	set (ALIMER_BINARY_DIR ${ALIMER_ROOT_DIR}/build/bin/${ALIMER_PLATFORM_NAME}/${ANDROID_ABI})
	set (ALIMER_LIB_DIR ${ALIMER_ROOT_DIR}/build/lib/${ALIMER_PLATFORM_NAME}/${ANDROID_ABI})
else()
	set (ALIMER_BINARY_DIR ${ALIMER_ROOT_DIR}/build/bin/${ALIMER_PLATFORM_NAME})
	set (ALIMER_LIB_DIR ${ALIMER_ROOT_DIR}/build/lib/${ALIMER_PLATFORM_NAME})
endif()

# Version
set(ALIMER_PRODUCT_NAME "Alimer")
set(ALIMER_PRODUCT_COMPANY "Amer Koleci")
set(ALIMER_PRODUCT_COPYRIGHT "Copyright (C) 2016-2017 ${ALIMER_PRODUCT_COMPANY}.")

set (ALIMER_VERSION_MAJOR "0")
set (ALIMER_VERSION_MINOR "9")
set (ALIMER_VERSION_PATCH "0")
set (ALIMER_ENGINE_VERSION "${ALIMER_VERSION_MAJOR}.${ALIMER_VERSION_MINOR}.${ALIMER_VERSION_PATCH}")

# Output directories.
set(CMAKE_ARCHIVE_OUTPUT_DIRECTORY ${ALIMER_LIB_DIR})
set(CMAKE_LIBRARY_OUTPUT_DIRECTORY ${ALIMER_LIB_DIR})
set(CMAKE_RUNTIME_OUTPUT_DIRECTORY ${ALIMER_BINARY_DIR})

if (NOT CMAKE_BUILD_TYPE)
	# If not specified then default to Release
    set (CMAKE_BUILD_TYPE Release)
endif ()

# Print some messages
message( "Engine version : ${ALIMER_ENGINE_VERSION}, root path : ${ALIMER_ROOT_DIR}" )
message( "CMAKE_BUILD_TYPE: ${CMAKE_BUILD_TYPE}" )

# External/ThirdParty

# Setup common include folders
include_directories(${ALIMER_INCLUDE_DIR})

# Alimer source
add_subdirectory (Source)