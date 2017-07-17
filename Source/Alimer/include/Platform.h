/*
** Alimer - Copyright (C) 2016-2017 Amer Koleci.
**
** This file is subject to the terms and conditions defined in
** file 'LICENSE', which is part of this source code package.
*/

#pragma once

#ifdef _MSC_VER
#	if _MSC_VER >= 1910		// Visual Studio 2017
#		define ALIMER_MSVC 15
#	elif _MSC_VER >= 1900	// Visual Studio 2015
#		define ALIMER_MSVC 14
#	elif _MSC_VER >= 1800	// Visual Studio 2013
#		define ALIMER_MSVC 12
#   elif _MSC_VER >= 1700	// Visual Studio 2012
#       define ALIMER_MSVC 11
#   elif _MSC_VER >= 1600	// Visual Studio 2010
#       define ALIMER_MSVC 10
#   elif _MSC_VER >= 1500	// Visual Studio 2008
#       define ALIMER_MSVC 9
#else
#	error "Unknown VC version"
#	endif

#elif defined(__clang__)
#	define ALIMER_CLANG 1
#elif defined(__SNC__)
#	define ALIMER_SNC 1
#elif defined(__ghs__)
#	define ALIMER_GHS 1
#elif defined(__GNUC__) // note: __clang__, __SNC__, or __ghs__ imply __GNUC__
#	define ALIMER_GCC 1
#else
#	error "Unknown compiler"
#endif

/**
* Operating system defines, see http://sourceforge.net/p/predef/wiki/OperatingSystems/
*/
#if defined(WINAPI_FAMILY) && WINAPI_FAMILY == WINAPI_PARTITION_APP
#	define ALIMER_WINMODERN 1 // Universal Windows platform
#elif defined(_XBOX_ONE) || defined(XBOXONE) || defined(_TITLE)
#	define ALIMER_XBOXONE 1
#elif defined(_WIN64) // note: XBOX_ONE implies _WIN64
#	define ALIMER_WIN64 1
#elif defined(_M_PPC)
#	define ALIMER_X360 1
#elif defined(_WIN32) // note: _M_PPC implies _WIN32
#	define ALIMER_WIN32 1
#elif defined(__ANDROID__)
#	define ALIMER_ANDROID 1
#elif defined(__linux__) || defined (__EMSCRIPTEN__) // note: __ANDROID__ implies __linux__
#	define ALIMER_LINUX 1
#elif defined(__APPLE__) && (defined(__arm__) || defined(__arm64__))
#	define ALIMER_IOS 1
#elif defined(__APPLE__)
#	define ALIMER_OSX 1
#elif defined(__ORBIS__)
#	define ALIMER_PS4 1
#endif

/**
* Architecture defines, see http://sourceforge.net/p/predef/wiki/Architectures/
*/
#if defined(__x86_64__) || defined(_M_X64) // ps4 compiler defines _M_X64 without value
#	define ALIMER_X64 1
#elif defined(__i386__) || defined(_M_IX86)
#	define ALIMER_X86 1
#elif defined(__arm64__) || defined(__aarch64__)
#	define ALIMER_A64 1
#elif defined(__arm__) || defined(_M_ARM)
#	define ALIMER_ARM 1
#elif defined(__SPU__)
#	define ALIMER_SPU 1
#elif defined(__ppc__) || defined(_M_PPC) || defined(__CELLOS_LV2__)
#	define ALIMER_PPC 1
#else
#	error "Unknown architecture"
#endif

/**
* SIMD defines
*/
#if defined(__i386__) || defined(_M_IX86) || defined(__x86_64__) || defined(_M_X64)
#	define ALIMER_SSE2 1
#endif
#if defined(_M_ARM) || defined(__ARM_NEON__)
#	define ALIMER_NEON 1
#endif
#if defined(_M_PPC) || defined(__CELLOS_LV2__)
#	define ALIMER_VMX 1
#endif

/**
* Set defaults for non defined defines.
*/
#ifndef ALIMER_MSVC
#	define ALIMER_MSVC 0
#endif
#ifndef ALIMER_CLANG
#	define ALIMER_CLANG 0
#endif
#ifndef ALIMER_SNC
#	define ALIMER_SNC 0
#endif
#ifndef ALIMER_GHS
#	define ALIMER_GHS 0
#endif
#ifndef ALIMER_GCC
#	define ALIMER_GCC 0
#endif
#ifndef ALIMER_WINMODERN
#	define ALIMER_WINMODERN 0
#endif
#ifndef ALIMER_XBOXONE
#	define ALIMER_XBOXONE 0
#endif
#ifndef ALIMER_WIN64
#	define ALIMER_WIN64 0
#endif
#ifndef ALIMER_X360
#	define ALIMER_X360 0
#endif
#ifndef ALIMER_WIN32
#	define ALIMER_WIN32 0
#endif
#ifndef ALIMER_ANDROID
#	define ALIMER_ANDROID 0
#endif
#ifndef ALIMER_LINUX
#	define ALIMER_LINUX 0
#endif
#ifndef ALIMER_IOS
#	define ALIMER_IOS 0
#endif
#ifndef ALIMER_OSX
#	define ALIMER_OSX 0
#endif
#ifndef ALIMER_PS4
#	define ALIMER_PS4 0
#endif
#ifndef ALIMER_X64
#	define ALIMER_X64 0
#endif
#ifndef ALIMER_X86
#	define ALIMER_X86 0
#endif
#ifndef ALIMER_A64
#	define ALIMER_A64 0
#endif
#ifndef ALIMER_ARM
#	define ALIMER_ARM 0
#endif
#ifndef ALIMER_SPU
#	define ALIMER_SPU 0
#endif
#ifndef ALIMER_PPC
#	define ALIMER_PPC 0
#endif
#ifndef ALIMER_SSE2
#	define ALIMER_SSE2 0
#endif
#ifndef ALIMER_NEON
#	define ALIMER_NEON 0
#endif
#ifndef ALIMER_VMX
#	define ALIMER_VMX 0
#endif

/**
* family shortcuts.
*/
// compiler
#define ALIMER_GCC_FAMILY (ALIMER_CLANG || ALIMER_GCC)
// os
#define ALIMER_WINDOWS_FAMILY (ALIMER_WIN32 || ALIMER_WIN64)
#define ALIMER_MICROSOFT_FAMILY (ALIMER_XBOXONE || ALIMER_WINDOWS_FAMILY)
#define ALIMER_LINUX_FAMILY (ALIMER_LINUX || ALIMER_ANDROID)
#define ALIMER_APPLE_FAMILY (ALIMER_IOS || ALIMER_OSX)
#define ALIMER_UNIX_FAMILY (ALIMER_LINUX_FAMILY || ALIMER_APPLE_FAMILY) // shortcut for unix/posix platforms

#if defined(__EMSCRIPTEN__)
#	define ALIMER_EMSCRIPTEN 1
#	define ALIMER_HTML5 1
#else
#	define ALIMER_EMSCRIPTEN 0
#	define ALIMER_HTML5 1
#endif
// architecture
#define ALIMER_INTEL_FAMILY (ALIMER_X64 || ALIMER_X86)
#define ALIMER_ARM_FAMILY (ALIMER_ARM || ALIMER_A64)
#define ALIMER_P64_FAMILY (ALIMER_X64 || ALIMER_A64) // shortcut for 64-bit architectures

/**
* DLL export macros
*/
#if !defined(ALIMER_C_EXPORT) 
#	if ALIMER_WINDOWS_FAMILY || ALIMER_LINUX
#		define ALIMER_C_EXPORT extern "C"
#	else
#		define ALIMER_C_EXPORT
#	endif
#endif

#if ALIMER_WINDOWS_FAMILY
#	define ALIMER_INTERFACE_EXPORT __declspec(dllexport)
#	define ALIMER_INTERFACE_IMPORT __declspec(dllimport)
#elif ALIMER_UNIX_FAMILY && __GNUC__ >= 4
#	define ALIMER_INTERFACE_EXPORT __attribute__((visibility("default")))
#	define ALIMER_INTERFACE_IMPORT
#else
#	define ALIMER_INTERFACE_EXPORT ALIMER_UNIX_EXPORT
#	define ALIMER_INTERFACE_IMPORT
#endif

/**
* Inline macro
*/
#define ALIMER_INLINE inline
#if ALIMER_MICROSOFT_FAMILY
#	pragma inline_depth( 255 )
#endif

/**
* Force inline macro
*/
#if ALIMER_MSVC
#	define ALIMER_FORCE_INLINE __forceinline
#elif ALIMER_LINUX
#	define ALIMER_FORCE_INLINE inline
#elif ALIMER_GCC_FAMILY
#	define ALIMER_FORCE_INLINE inline __attribute__((always_inline))
#else
#	define ALIMER_FORCE_INLINE inline
#endif

// Assert macros.
#ifdef _DEBUG
#ifdef _WIN32
#include <crtdbg.h>
#	define ALIMER_ASSERT(expr) _ASSERT((expr));
#	define ALIMER_ASSERT_MSG(expr, msg) _ASSERT_EXPR((expr), msg);
#else
#	define ALIMER_ASSERT(expr) assert(expr)
#	define ALIMER_ASSERT_MSG(expr, msg) assert(expr && msg)
#endif

#else
#	define ALIMER_ASSERT(expr)
#	define ALIMER_ASSERT_MSG(expr, msg)
#endif

#if ALIMER_EXPORTS
#	define ALIMER_API ALIMER_INTERFACE_EXPORT
#else
#	define ALIMER_API ALIMER_INTERFACE_IMPORT
#endif

// Misc
#if defined(__COVERITY__)
#	define ALIMER_UNUSED(x) ((void)(x))
#elif defined( __GNUC__ )
#	define ALIMER_UNUSED(x) ((void)sizeof((x)))
#else
#	define ALIMER_UNUSED(x) (/*lint --e{505,550,818,866} */(void)sizeof((x), 0))
#endif

#define ALIMER_STRINGIZE_HELPER(X) #X
#define ALIMER_STRINGIZE(X) ALIMER_STRINGIZE_HELPER(X)

#define ALIMER_CONCAT_HELPER(X, Y) X##Y
#define ALIMER_CONCAT(X, Y) ALIMER_CONCAT_HELPER(X, Y)

// Include common types header.
#include "Types.h"
