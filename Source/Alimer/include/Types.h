/*
** Alimer - Copyright (C) 2016-2017 Amer Koleci.
**
** This file is subject to the terms and conditions defined in
** file 'LICENSE.md', which is part of this source code package.
*/

#pragma once

#include <cstdint>

typedef std::uint64_t	uint64;
typedef std::uint32_t	uint32;
typedef std::uint16_t	uint16;
typedef std::uint8_t	uint8;

typedef std::int64_t	int64;
typedef std::int32_t	int32;
typedef std::int16_t	int16;
typedef std::int8_t		int8;

typedef wchar_t			wchar;

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

namespace Alimer
{
	static const size_t CONVERSION_BUFFER_LENGTH = 256;
	static const size_t MATRIX_CONVERSION_BUFFER_LENGTH = 256;

	typedef std::wstring WString;
	typedef std::string String;
	typedef std::wstringstream WStringStream;
	typedef std::stringstream StringStream;
}

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