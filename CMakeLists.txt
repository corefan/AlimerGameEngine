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
set (ALIMER_BINARY_DIR ${ALIMER_ROOT_DIR}/build/bin/${ALIMER_PLATFORM_NAME})
set (ALIMER_LIB_DIR ${ALIMER_ROOT_DIR}/build/lib/${ALIMER_PLATFORM_NAME})

# Set CMake modules search path
set(CMAKE_MODULE_PATH ${CMAKE_MODULE_PATH} ${CMAKE_SOURCE_DIR}/CMake/Modules)
# load required Alimer cmake macros
include(Alimer REQUIRED)