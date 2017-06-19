cmake_minimum_required(VERSION 3.8)

# use include guard.
if( _CMAKE_ALIMER_GUARD_ )
	return()
endif()
set(_CMAKE_ALIMER_GUARD_ 1)

include(AlimerMacros)

# set host system variables
set (ALIMER_HOST_WINDOWS 0)
set(ALIMER_HOST_OSX 0)
set(ALIMER_HOST_LINUX 0)

if (${CMAKE_HOST_SYSTEM_NAME} STREQUAL "Windows")
	set(ALIMER_HOST_WINDOWS 1)
	message("Alimer host system: Windows")
elseif (${CMAKE_HOST_SYSTEM_NAME} STREQUAL "Darwin")
	set(ALIMER_HOST_OSX 1)
	message("Alimer host system: OSX")
elseif (${CMAKE_HOST_SYSTEM_NAME} STREQUAL "Linux")
	set(ALIMER_HOST_LINUX 1)
	message("Alimer host system: Linux")         
endif()

# detect compiler and set variables
message("CMAKE_CXX_COMPILER_ID: ${CMAKE_CXX_COMPILER_ID}")
if (${CMAKE_CXX_COMPILER_ID} MATCHES "Clang")
	set(ALIMER_CLANG 1)
	message("Detected C++ Compiler: Clang (ALIMER_CLANG)")
elseif (${CMAKE_CXX_COMPILER_ID} MATCHES "GNU")
	set(ALIMER_GCC 1)
	message("Detected C++ Compiler: GCC (ALIMER_GCC)")
elseif (MSVC)
	set(ALIMER_MSVC 1)
	message("Detected C++ Compiler: VStudio (ALIMER_MSVC)")
else()
	if (ALIMER_EMSCRIPTEN)
    		set(ALIMER_CLANG 1)
    		message("Detect C++ Compiler by platform: Clang (ALIMER_CLANG)")
	elseif (ALIMER_ANDROID)
    		set(ALIMER_GCC 1)
    		message("Detect C++ Compiler by platform: GCC (ALIMER_GCC)")
	else()
    		message("Detected C++ Compiler: Unknown")
	endif()
endif()

# Detect target platform
set(ALIMER_PLATFORM_64BIT 0)
if( ${CMAKE_SYSTEM_NAME} STREQUAL "Windows" )
	set(ALIMER_PLATFORM_WINDOWS 1)
	set(ALIMER_PLATFORM_NAME "windows")
	
	if (CMAKE_CL_64)
		set(ALIMER_PLATFORM_64BIT 1)
		set(ALIMER_WIN64 1)
		set(ALIMER_WINDOWS_PLATFORM_NAME "x64")
	else()
		set(ALIMER_WIN32 1)
		set(ALIMER_WINDOWS_PLATFORM_NAME "x86")
	endif()
	
elseif( ${CMAKE_SYSTEM_NAME} STREQUAL "WindowsStore" )
	set(ALIMER_PLATFORM_UWP 1)
	set(ALIMER_PLATFORM_NAME "UWP")
	set(ALIMER_UWP_VERSION_MIN 10.0.10586.0)
	set(ALIMER_UWP_VERSION_TARGET 10.0.10586.0)
	if (CMAKE_CL_64)
		set(ALIMER_PLATFORM_64BIT 1)
	endif()
		
elseif( ${CMAKE_SYSTEM_NAME} STREQUAL "Darwin" )
	if( IOS )
		set(ALIMER_PLATFORM_IOS 1)
		set(ALIMER_PLATFORM_NAME "ios")
		if(CMAKE_OSX_ARCHITECTURES MATCHES "arm64")
			set(ALIMER_PLATFORM_64BIT 1)
		endif()
	else()
		set(ALIMER_PLATFORM_OSX 1)
		set(ALIMER_PLATFORM_NAME "osx")
		
		if (CMAKE_CL_64)
			set(ALIMER_PLATFORM_64BIT 1)
		endif()
	endif()
elseif( ${CMAKE_SYSTEM_NAME} STREQUAL "Linux" )
	set(ALIMER_PLATFORM_LINUX 1)
	set(ALIMER_PLATFORM_NAME "linux")
	set(ALIMER_PLATFORM_64BIT 1)
elseif( ${CMAKE_SYSTEM_NAME} STREQUAL "Android" )
	set(ALIMER_PLATFORM_ANDROID 1)
	set(ALIMER_PLATFORM_NAME "android")
elseif( ${CMAKE_SYSTEM_NAME} STREQUAL "Durango" )
	set(ALIMER_PLATFORM_XBOXONE 1)
	set(ALIMER_PLATFORM_NAME "xb1")
	set(ALIMER_PLATFORM_64BIT 1)
elseif( ${CMAKE_SYSTEM_NAME} STREQUAL "Orbis" )
	set(ALIMER_PLATFORM_PS4 1)
	set(ALIMER_PLATFORM_NAME "ps4")
	set(ALIMER_PLATFORM_64BIT 1)
elseif( ${CMAKE_SYSTEM_NAME} STREQUAL "Emscripten" )
	set(ALIMER_PLATFORM_WEB 1)
	set(ALIMER_PLATFORM_NAME "web")
else()
	message(FATAL_ERROR "Unknown platform ${CMAKE_SYSTEM_NAME}")
endif()

message(STATUS "Target platform: ${ALIMER_PLATFORM_NAME}")


# Detect target architecture
if( ALIMER_PLATFORM_WINDOWS OR ALIMER_PLATFORM_OSX OR ALIMER_PLATFORM_LINUX OR ALIMER_PLATFORM_XBOXONE OR ALIMER_PLATFORM_PS4 OR ALIMER_PLATFORM_WEB OR ALIMER_PLATFORM_UWP )
	if( ALIMER_PLATFORM_64BIT )
		set(ARCH_NAME "x64")
	else()
		set(ARCH_NAME "x86")
	endif()
elseif( ALIMER_PLATFORM_IOS OR ALIMER_PLATFORM_ANDROID )
	if( ALIMER_PLATFORM_64BIT )
		set(ARCH_NAME "arm64")
	else()
		set(ARCH_NAME "arm")
	endif()
else()
	message(FATAL_ERROR "Unknown platform architecture!")
endif()

message(STATUS "Detected architecture: ${ARCH_NAME}")

set(ALIMER_LIB_PREFIX ${CMAKE_STATIC_LIBRARY_PREFIX})
set(ALIMER_LIB_SUFFIX ${CMAKE_STATIC_LIBRARY_SUFFIX})

# Configure CMake global variables
set(CMAKE_INSTALL_MESSAGE LAZY)
SET(CMAKE_ARCHIVE_OUTPUT_DIRECTORY ${ALIMER_LIB_DIR})
SET(CMAKE_LIBRARY_OUTPUT_DIRECTORY ${ALIMER_LIB_DIR})
SET(CMAKE_RUNTIME_OUTPUT_DIRECTORY ${ALIMER_BINARY_DIR})
if( ALIMER_PLATFORM_WINDOWS )
	set(CMAKE_VS_INCLUDE_INSTALL_TO_DEFAULT_BUILD 1)
endif()

# Enable solution explorer folders.
set (ALIMER_USE_SOLUTION_FOLDERS ON)

# Define standard configurations
set (ALIMER_BUILD_CONFIGURATIONS Debug Dev Release)
set (DOC_STRING "Specify CMake build configuration (single-configuration generator only), possible values are Release (default), Dev, and Debug")
if (CMAKE_CONFIGURATION_TYPES)
    # For multi-configurations generator, such as VS and Xcode
    set (CMAKE_CONFIGURATION_TYPES ${ALIMER_BUILD_CONFIGURATIONS} CACHE STRING ${DOC_STRING} FORCE)
    unset (CMAKE_BUILD_TYPE)
else ()
    # For single-configuration generator, such as Unix Makefile generator
    if (CMAKE_BUILD_TYPE STREQUAL "")
        # If not specified then default to Release
        set (CMAKE_BUILD_TYPE Release)
    endif ()
    set (CMAKE_BUILD_TYPE ${CMAKE_BUILD_TYPE} CACHE STRING ${DOC_STRING} FORCE)
endif ()

include (CMakeDependentOption)

# Set definitions
if( ALIMER_PLATFORM_WINDOWS OR ALIMER_PLATFORM_ANDROID OR ALIMER_PLATFORM_IOS)
	set(ALIMER_SHARED ON CACHE BOOL "Build Alimer as a shared library.")
	set(ALIMER_LIB_TYPE SHARED)
else()
	set(ALIMER_SHARED OFF CACHE BOOL "Build Alimer as a shared library.")
	set(ALIMER_LIB_TYPE STATIC)
endif()

if(ALIMER_USE_SOLUTION_FOLDERS)
	set_property(GLOBAL PROPERTY USE_FOLDERS ON)
endif()

# Compiler-specific setup
if (MSVC)
	if (ALIMER_STATIC_RUNTIME)
		set (RELEASE_RUNTIME /MT)
		set (DEBUG_RUNTIME /MTd)
	endif ()

	set (CMAKE_C_FLAGS_DEBUG "${CMAKE_C_FLAGS_DEBUG} ${DEBUG_RUNTIME}")
	set (CMAKE_C_FLAGS_DEV "${CMAKE_C_FLAGS_RELEASE} ${RELEASE_RUNTIME} /fp:fast /Zi /GS-")
	set (CMAKE_C_FLAGS_RELEASE "${CMAKE_C_FLAGS_DEV}")
	set (CMAKE_CXX_FLAGS_DEBUG "${CMAKE_CXX_FLAGS_DEBUG} ${DEBUG_RUNTIME} /W4")
	set (CMAKE_CXX_FLAGS_DEV "${CMAKE_CXX_FLAGS_RELEASE} ${RELEASE_RUNTIME} /W4 /fp:fast /Zi /GS- ")
	set (CMAKE_CXX_FLAGS_RELEASE "${CMAKE_CXX_FLAGS_DEV}")
	set (CMAKE_SHARED_LINKER_FLAGS_DEV "${CMAKE_MODULE_LINKER_FLAGS_RELEASE}")
	set (CMAKE_EXE_LINKER_FLAGS_DEV "${CMAKE_EXE_LINKER_FLAGS_RELEASE} /OPT:REF /OPT:ICF /DEBUG")
	set (CMAKE_EXE_LINKER_FLAGS_RELEASE "${CMAKE_EXE_LINKER_FLAGS_RELEASE} /OPT:REF /OPT:ICF")
	
	# Enable multiprocessor compile
	if (MSVC_VERSION GREATER 1500 OR MSVC_VERSION EQUAL 1500)
		set(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} /MP")
	endif()
	
	if(MSVC_VERSION GREATER 1400 OR MSVC_VERSION EQUAL 1400)
		 set(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} /Zm256")
	endif()

	# Disable (useless) compiler warnings on project level 
	add_definitions( /wd4786 /wd4503 /wd4251 /wd4275 /wd4290 /wd4661 /wd4996 /wd4127 /wd4100 /wd4702 /wd4201 /wd4310)

endif()