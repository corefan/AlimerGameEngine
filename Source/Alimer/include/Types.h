/*
** Alimer - Copyright (C) Amer Koleci
**
** This file is subject to the terms and conditions defined in
** file 'LICENSE.txt', which is part of this source code package.
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
#include <iostream>
#include <sstream>
#include <string>
#include <array>
#include <vector>

namespace Alimer
{
	static const size_t CONVERSION_BUFFER_LENGTH = 256;
	static const size_t MATRIX_CONVERSION_BUFFER_LENGTH = 256;

	typedef std::wstring WString;
	typedef std::string String;
	typedef std::wstringstream WStringStream;
	typedef std::stringstream StringStream;
}