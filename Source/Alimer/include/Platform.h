/*
** Alimer - Copyright (C) 2016-2017 Amer Koleci.
**
** This file is subject to the terms and conditions defined in
** file 'LICENSE.md', which is part of this source code package.
*/

#pragma once

#ifdef _MSC_VER
#	define ALIMER_MSVC

#	if _MSC_VER >= 1910		// Visual Studio 2017
#		define ALIMER_MSVC15
#	elif _MSC_VER >= 1900	// Visual Studio 2015
#		define ALIMER_MSVC14
#	elif _MSC_VER >= 1800	// Visual Studio 2013
#		define ALIMER_MSVC12
#   elif _MSC_VER >= 1700	// Visual Studio 2012
#       define ALIMER_MSVC11
#   elif _MSC_VER >= 1600	// Visual Studio 2010
#       define ALIMER_MSVC10
#   elif _MSC_VER >= 1500	// Visual Studio 2008
#       define ALIMER_MSVC9
#	elif _MSC_VER >= 1400
#		define ALIMER_MSVC8
#	elif _MSC_VER >= 1300
#		define ALIMER_MSVC7
#	else
#		define ALIMER_MSVC6
#	endif

#elif __GNUC__ || __SNC__
#	define ALIMER_GNUC
#	ifdef __clang__
#		define ALIMER_CLANG
#	endif
#else
#	error "Unknown compiler"
#endif

/**
* Platform define
*/
#ifdef ALIMER_MSVC
#	if defined(_XBOX_ONE) || defined(XBOXONE) || defined(_TITLE)
#		define ALIMER_XBOXONE 1
#		define ALIMER_X64 1
#	elif defined(_M_IX86)
#		define ALIMER_X86 1
#		define ALIMER_WINDOWS 1
#		define ALIMER_SSE 1
#   elif defined(_M_X64)
#       define ALIMER_X64 1
#       define ALIMER_WINDOWS 1
#		define ALIMER_SSE 1
#	elif defined(_M_PPC)
#		define ALIMER_PPC 1
#		define ALIMER_X360 1
#		define ALIMER_VMX 1
#   elif defined(_M_ARM)
#		define ALIMER_ARM 1
#       define ALIMER_ARM_NEON 1
#	else
#		error "Unknown platform"
#	endif
#	if defined(WINAPI_FAMILY) && (WINAPI_FAMILY == WINAPI_PARTITION_APP)
#		define ALIMER_WINMODERN 1
#	endif
#elif defined ALIMER_GNUC
#   ifdef __CELLOS_LV2__
#		define ALIMER_PS3 1
#		define ALIMER_VMX 1
#   elif defined(__arm__)
#		define ALIMER_ARM 1
#		if defined(__SNC__)
#			define ALIMER_PSP2 1
#		endif
#		if defined(__ARM_NEON__)
#			define ALIMER_ARM_NEON 1
#		endif
#   elif defined(__arm64__)
#		define ALIMER_A64
#		if defined(__ARM_NEON__)
#			define ALIMER_ARM_NEON 1
#			define ALIMER_ARM_NEON64 1
#		endif
#   elif defined(__i386__)
#       define ALIMER_X86 1
#		define ALIMER_VMX 1
#   elif defined(__x86_64__)
#       ifdef __ORBIS__
#         define ALIMER_PS4 1
#		  define ALIMER_X64 1
#       else
#       define ALIMER_X64 1
#       endif
#   elif defined(__ppc__)
#       define ALIMER_PPC 1
#   elif defined(__ppc64__)
#       define ALIMER_PPC 1
#		define ALIMER_PPC64 1
#   elif defined(__mips64) || defined(__mips64_)
#       define ALIMER_MIPS 1
#		define ALIMER_MIPS64 1
#	elif defined (EMSCRIPTEN)
#       define ALIMER_HTML5 1
#       define ALIMER_EMSCRIPTEN 1
#   else
#	error "Unknown platform"
#   endif
#	if defined(__ANDROID__)
#   	define ALIMER_ANDROID 1
#   	define ALIMER_UNIX 1
#	elif defined(__linux__)
#   	define ALIMER_LINUX 1
#   	define ALIMER_UNIX 1
#	elif defined(__APPLE__)
#   	define ALIMER_APPLE 1
#   	define ALIMER_UNIX 1
#		include <TargetConditionals.h>
#		if TARGET_OS_IOS
#			define ALIMER_IOS 1
#		elif TARGET_OS_TV
#			define ALIMER_TVOS 1
#		elif TARGET_OS_MAC
#			define ALIMER_MACOS 1
#		endif
#	elif defined(__CYGWIN__)
#   	define ALIMER_CYGWIN 1
#   	define ALIMER_LINUX 1
#   	define ALIMER_UNIX 1
#	endif
#endif

/**
* DLL export macros
*/
#if !defined(ALIMER_C_EXPORT) 
#	if defined(ALIMER_WINDOWS) || defined(ALIMER_WINMODERN) 
#		define ALIMER_C_EXPORT extern "C"
#	else
#		define ALIMER_C_EXPORT
#	endif
#endif

#if defined(__CYGWIN32__)
#	define ALIMER_INTERFACE_API __stdcall
#	define ALIMER_INTERFACE_EXPORT __declspec(dllexport)
#	define ALIMER_INTERFACE_IMPORT __declspec(dllimport)

#elif defined(WIN32) || defined(_WIN32) || defined(__WIN32__) || defined(_WIN64) || defined(WINAPI_FAMILY)
#	define ALIMER_INTERFACE_API __stdcall
#	define ALIMER_INTERFACE_EXPORT __declspec(dllexport)
#	define ALIMER_INTERFACE_IMPORT __declspec(dllimport)
#elif defined(__MACH__) || defined(__ANDROID__) || defined(__linux__) || defined(__QNX__)
#	define ALIMER_INTERFACE_API
#	define ALIMER_INTERFACE_EXPORT
#	define ALIMER_INTERFACE_IMPORT
#else
#	define ALIMER_INTERFACE_API
#	define ALIMER_INTERFACE_EXPORT
#	define ALIMER_INTERFACE_IMPORT
#endif

/**
* Inline macro
*/
#if defined(ALIMER_WINDOWS) || defined(ALIMER_X360) || defined(ALIMER_WINMODERN) || defined(ALIMER_XBOXONE)
#	define ALIMER_INLINE inline
#	pragma inline_depth( 255 )
#else
#	define ALIMER_INLINE inline
#endif

/**
* Force inline macro
*/
#if defined(ALIMER_MSVC)
#	define ALIMER_FORCE_INLINE __forceinline
#elif defined(ALIMER_LINUX) // Workaround; Fedora Core 3 do not agree with force inline and PxcPool
#	define ALIMER_FORCE_INLINE inline
#elif defined(ALIMER_GNUC) || defined(ALIMER_GHS)
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

#define ALIMER_STRINGIZE_HELPER(X) #X
#define ALIMER_STRINGIZE(X) ALIMER_STRINGIZE_HELPER(X)

#define ALIMER_CONCAT_HELPER(X, Y) X##Y
#define ALIMER_CONCAT(X, Y) ALIMER_CONCAT_HELPER(X, Y)

#include "Types.h"

namespace Alimer
{
	class ALIMER_API Platform
	{
	public:
		/**
		* Cross platform logging.
		*/
		static void Print(const char* format, ...);

	private:
		Platform() {}
		virtual ~Platform() { }

		Platform(const Platform&) = delete;
		Platform& operator=(const Platform&) = delete;
	};
}