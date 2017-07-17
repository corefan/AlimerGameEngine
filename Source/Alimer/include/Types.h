/*
** Alimer - Copyright (C) 2016-2017 Amer Koleci.
**
** This file is subject to the terms and conditions defined in
** file 'LICENSE', which is part of this source code package.
*/

#pragma once

#include <stdbool.h>
#include <stdarg.h>
#include <stdint.h>
#include <stddef.h>
#include <stdlib.h>
#include <float.h>
#include <limits.h>
#if !ALIMER_WINDOWS_FAMILY
#  include <wchar.h>
#endif

#if ( ALIMER_LINUX )
#  include <sys/types.h>
#endif

#ifndef __cplusplus
#  define nullptr ((void*)0)
#endif

typedef wchar_t			wchar;

/* Hash value */
typedef uint64_t		hash_t;

// Pointer arithmetic
#define pointer_offset(ptr, ofs) (void*)((char*)(ptr) + (ptrdiff_t)(ofs))
#define pointer_offset_const(ptr, ofs) (const void*)((const char*)(ptr) + (ptrdiff_t)(ofs))
#define pointer_diff(first, second) (ptrdiff_t)((const char*)(first) - (const char*)(second))

#define ALIMER_DEFINE_HANDLE(object) typedef struct object##_T* object

#ifdef __cplusplus

template <typename T, unsigned int N>
inline unsigned int ArraySize(T(&)[N])
{
	return N;
}

template <typename T>
void SafeDelete(T*& resource)
{
	delete resource;
	resource = nullptr;
}
template <typename T>
void SafeDeleteArray(T*& resource)
{
	delete[] resource;
	resource = nullptr;
}

#include <new>
#include <memory>
#include <cstdio>
#include <cstdlib>
#include <cassert>
#include <cwchar>
#include <cwctype>
#include <cctype>
#include <cmath>
#include <cstdarg>
#include <ctime>
#include <iostream>
#include <sstream>
#include <string>
#include <cstring>
#include <array>
#include <vector>
#include <list>
#include <set>
#include <stack>
#include <map>
#include <unordered_map>
#include <queue>
#include <bitset>
#include <algorithm>
#include <utility>
#include <limits>
#include <functional>
#include <typeinfo>
#include <thread>
#include <atomic>
#include <mutex>
#include <chrono>
#include <typeindex>

#endif // __cplusplus

#if ALIMER_WINDOWS_FAMILY
#   ifndef STRICT
#       define STRICT 1
#   endif
#   ifndef WIN32_LEAN_AND_MEAN
#       define WIN32_LEAN_AND_MEAN 1
#   endif
#   ifndef NOMINMAX
#       define NOMINMAX 1
#   endif

//#	define NODRAWTEXT
//#	define NOGDI
//#	define NOBITMAP
#	define NOMCX
#	define NOSERVICE
#	define NOHELP

#	include <objbase.h>
#	include <windows.h>
#	include <windowsx.h>
#	include <WinSock2.h>
#	include <IPTypes.h>
#	include <WS2tcpip.h>
#	include <iphlpapi.h>
#	include <share.h>
#	include <io.h>
#	include <shellapi.h>
#	include <shellscalingapi.h>
#	include <stdlib.h>
#	include <ShlObj.h>
#	include <DbgHelp.h>
#	include <crtdbg.h>
#	include <wrl.h>

using Microsoft::WRL::ComPtr;

#endif

#ifdef __cplusplus

// A macro to disallow the copy constructor and operator= functions.
// This should be used in the private: declarations for a class.
#define DISALLOW_COPY_AND_ASSIGN(TypeName) \
    TypeName(const TypeName &) = delete;   \
    void operator=(const TypeName &) = delete

// Utility to enable bitmask operators on enum classes.
// To use define an enum class with valid bitmask values and an underlying type
// then use the macro to enable support:
//  enum class MyFlagBits : uint32_t {
//    Count1 = 1 << 0,
//    Count2 = 1 << 1,
//  };
//  ALIMER_BITMASK(MyFlagBits);
#define ALIMER_BITMASK(enum_class)                                       \
  inline enum_class operator|(enum_class lhs, enum_class rhs) {        \
    typedef typename std::underlying_type<enum_class>::type enum_type; \
    return static_cast<enum_class>(static_cast<enum_type>(lhs) |       \
                                   static_cast<enum_type>(rhs));       \
  }                                                                    \
  inline enum_class& operator|=(enum_class& lhs, enum_class rhs) {     \
    typedef typename std::underlying_type<enum_class>::type enum_type; \
    lhs = static_cast<enum_class>(static_cast<enum_type>(lhs) |        \
                                  static_cast<enum_type>(rhs));        \
    return lhs;                                                        \
  }                                                                    \
  inline enum_class operator&(enum_class lhs, enum_class rhs) {        \
    typedef typename std::underlying_type<enum_class>::type enum_type; \
    return static_cast<enum_class>(static_cast<enum_type>(lhs) &       \
                                   static_cast<enum_type>(rhs));       \
  }                                                                    \
  inline enum_class& operator&=(enum_class& lhs, enum_class rhs) {     \
    typedef typename std::underlying_type<enum_class>::type enum_type; \
    lhs = static_cast<enum_class>(static_cast<enum_type>(lhs) &        \
                                  static_cast<enum_type>(rhs));        \
    return lhs;                                                        \
  }                                                                    \
  inline enum_class operator^(enum_class lhs, enum_class rhs) {        \
    typedef typename std::underlying_type<enum_class>::type enum_type; \
    return static_cast<enum_class>(static_cast<enum_type>(lhs) ^       \
                                   static_cast<enum_type>(rhs));       \
  }                                                                    \
  inline enum_class& operator^=(enum_class& lhs, enum_class rhs) {     \
    typedef typename std::underlying_type<enum_class>::type enum_type; \
    lhs = static_cast<enum_class>(static_cast<enum_type>(lhs) ^        \
                                  static_cast<enum_type>(rhs));        \
    return lhs;                                                        \
  }                                                                    \
  inline enum_class operator~(enum_class lhs) {                        \
    typedef typename std::underlying_type<enum_class>::type enum_type; \
    return static_cast<enum_class>(~static_cast<enum_type>(lhs));      \
  }

namespace Alimer
{
	static const size_t CONVERSION_BUFFER_LENGTH = 256;
	static const size_t MATRIX_CONVERSION_BUFFER_LENGTH = 256;

	typedef std::wstring WString;
	typedef std::string String;
	typedef std::wstringstream WStringStream;
	typedef std::stringstream StringStream;
}

#endif // __cplusplus

#if defined(_WIN32)
#pragma warning( disable : 4005 )
#pragma warning( disable : 4172 )
#pragma warning( disable : 4244 )
#pragma warning( disable : 4267 )
#pragma warning( disable : 4302 )
#pragma warning( disable : 4311 )
#pragma warning( disable : 4312 )
#pragma warning( disable : 4316 )
#pragma warning( disable : 4390 )
#pragma warning( disable : 4800 )
#pragma warning( disable : 4996 )
#endif