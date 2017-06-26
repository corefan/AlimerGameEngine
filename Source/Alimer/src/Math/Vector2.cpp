/*
** Alimer - Copyright (C) Amer Koleci
**
** This file is subject to the terms and conditions defined in
** file 'LICENSE.md', which is part of this source code package.
*/

#include "Math/Vector2.h"
#include "Math/Math.h"

namespace Alimer
{
	const Vector2 Vector2::ZERO;
	const Vector2 Vector2::LEFT(-1.0f, 0.0f);
	const Vector2 Vector2::RIGHT(1.0f, 0.0f);
	const Vector2 Vector2::UP(0.0f, 1.0f);
	const Vector2 Vector2::DOWN(0.0f, -1.0f);
	const Vector2 Vector2::ONE(1.0f, 1.0f);
	const Vector2 Vector2::UNIT_X(1.0f, 0.0f);
	const Vector2 Vector2::UNIT_Y(0.0f, 1.0f);

	Vector2::Vector2()
		: x(0.0f), y(0.0f)
	{
	}

	Vector2::Vector2(float value)
		: x(value), y(value)
	{

	}

	Vector2::Vector2(float x_, float y_)
		: x(x_), y(y_)
	{
	}

	Vector2::Vector2(const float* floatArray)
	{
		ALIMER_ASSERT(floatArray);

		x = floatArray[0];
		y = floatArray[1];
	}

	Vector2::Vector2(const Vector2& copy)
		: x(copy.x), y(copy.y)
	{
	}

	Vector2::~Vector2()
	{
	}

	bool Vector2::IsZero() const
	{
		return x == 0.0f && y == 0.0f;
	}

	bool Vector2::IsOne() const
	{
		return x == 1.0f && y == 1.0f;
	}

	void Vector2::Set(float x_, float y_)
	{
		this->x = x_;
		this->y = y_;
	}

	void Vector2::Set(const float* floatArray)
	{
		ALIMER_ASSERT(floatArray);

		x = floatArray[0];
		y = floatArray[1];
	}

	void Vector2::Set(const Vector2& v)
	{
		this->x = v.x;
		this->y = v.y;
	}

	float Vector2::Length() const 
	{
		return std::sqrt(x * x + y * y);
	}

	float Vector2::LengthSquared() const 
	{
		return x * x + y * y;
	}

	float Vector2::Distance(const Vector2& v1, const Vector2& v2)
	{
		float dx = v1.x - v2.x;
		float dy = v1.y - v2.y;

		return sqrt(dx * dx + dy * dy);
	}

	float Vector2::DistanceSquared(const Vector2& v1, const Vector2& v2)
	{
		float dx = v1.x - v2.x;
		float dy = v1.y - v2.y;

		return (dx * dx + dy * dy);
	}

	float Vector2::Dot(const Vector2& vector) const
	{
		return (x * vector.x + y * vector.y);
	}

	float Vector2::Dot(const Vector2& v1, const Vector2& v2)
	{
		return (v1.x * v2.x + v1.y * v2.y);
	}

	void Vector2::Lerp(const Vector2& start, const Vector2& end, float amount, Vector2* result)
	{
		ALIMER_ASSERT(result);

		result->x = start.x + ((end.x - start.x) * amount);
		result->y = start.y + ((end.y - start.y) * amount);
	}

	Vector2 Vector2::Lerp(const Vector2& start, const Vector2& end, float amount)
	{
		Vector2 result;
		result.x = start.x + ((end.x - start.x) * amount);
		result.y = start.y + ((end.y - start.y) * amount);
		return result;
	}

	String Vector2::ToString() const
	{
		char tempBuffer[CONVERSION_BUFFER_LENGTH];
		sprintf(tempBuffer, "%g %g", x, y);
		return String(tempBuffer);
	}

	uint32_t Vector2::ToHash() const
	{
		uint32_t hashCode = FloatToRawIntBits(x);
		hashCode = (hashCode * 397) ^ FloatToRawIntBits(y);
		return hashCode;
	}
}