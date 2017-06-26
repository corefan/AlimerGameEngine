/*
** Alimer - Copyright (C) Amer Koleci
**
** This file is subject to the terms and conditions defined in
** file 'LICENSE.md', which is part of this source code package.
*/

#pragma once

#include "Prerequisites.h"

namespace Alimer
{
#undef M_PI
	static const float M_PI = 3.14159265358979323846264338327950288f;

	/// Return a representation of the specified floating-point value as a single format bit layout.
	inline uint32_t FloatToRawIntBits(float value)
	{
		uint32_t u = *((uint32_t*)&value);
		return u;
	}

	/// Check whether a floating point value is NaN.
	/// Use a workaround for GCC, see https://github.com/urho3d/Urho3D/issues/655
#ifndef __GNUC__
	inline bool IsNaN(float value) { return value != value; }
#else

	inline bool IsNaN(float value)
	{
		uint32_t u = FloatToRawIntBits(value);
		return (u & 0x7fffffff) > 0x7f800000;
	}

#endif
}
