/*
** Alimer - Copyright (C) Amer Koleci
**
** This file is subject to the terms and conditions defined in
** file 'LICENSE.md', which is part of this source code package.
*/

#include "Math/Rect.h"
#include "Math/Math.h"

namespace Alimer
{
	const IntRect IntRect::EMPTY(0, 0, 0, 0);

	IntRect::IntRect()
		: x(0), y(0), width(0), height(0)
	{
	}

	IntRect::IntRect(uint32_t width_, uint32_t height_)
		: x(0), y(0), width(width_), height(height_)
	{
	}

	IntRect::IntRect(int x_, int y_, uint32_t width_, uint32_t height_)
		: x(x_), y(y_), width(width_), height(height_)
	{
	}

	IntRect::IntRect(const IntRect& copy)
		: x(copy.x), y(copy.y), width(copy.width), height(copy.height)
	{
	}

	IntRect::~IntRect()
	{
	}

	bool IntRect::IsEmpty() const
	{
		return (x == 0 && y == 0 && width == 0 && height == 0);
	}

	String IntRect::ToString() const
	{
		char tempBuffer[CONVERSION_BUFFER_LENGTH];
		sprintf(tempBuffer, "%d %d %d %d", x, y, width, height);
		return String(tempBuffer);
	}

	uint32_t IntRect::ToHash() const
	{
		uint32_t hashCode = x;
		hashCode = (hashCode * 397) ^ y;
		hashCode = (hashCode * 397) ^ width;
		hashCode = (hashCode * 397) ^ height;
		return hashCode;
	}
}