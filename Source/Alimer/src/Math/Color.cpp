/*
** Alimer - Copyright (C) Amer Koleci
**
** This file is subject to the terms and conditions defined in
** file 'LICENSE', which is part of this source code package.
*/

#include "Math/Color.h"
#include "Math/Math.h"
#include "Utils/StringUtils.h"

namespace Alimer
{
	Color::Color()
		: r(0.0f), g(0.0f), b(0.0f), a(0.0f)
	{
	}

	Color::Color(float r_, float g_, float b_)
		: r(r_), g(g_), b(b_), a(1.0f)
	{
	}

	Color::Color(float r_, float g_, float b_, float a_)
		: r(r_), g(g_), b(b_), a(a_)
	{
	}

	Color::Color(const float* floatArray)
	{
		Set(floatArray);
	}

	Color::Color(const Color& copy)
	{
		Set(copy);
	}

	Color::Color(const Color& color, float alpha)
		: r(color.r), g(color.g), b(color.b), a(alpha)
	{

	}

	Color::Color(const ColorBGRA& bgraColor)
	{
		Set(bgraColor);
	}

	Color::Color(const ColorRGBA& rgbaColor)
	{
		Set(rgbaColor);
	}

	Color::~Color()
	{
	}

	void Color::Set(float r_, float g_, float b_, float a_)
	{
		this->r = r_;
		this->g = g_;
		this->b = b_;
		this->a = a_;
	}

	void Color::Set(const float* floatArray)
	{
		ALIMER_ASSERT(floatArray);

		r = floatArray[0];
		g = floatArray[1];
		b = floatArray[2];
		a = floatArray[3];
	}

	void Color::Set(const Color& copy)
	{
		this->r = copy.r;
		this->g = copy.g;
		this->b = copy.b;
		this->a = copy.a;
	}

	void Color::Set(const ColorBGRA& bgraColor)
	{
		int32_t iColor = (int32_t)(bgraColor.c);
		r = (float)((iColor >> 16) & 0xFF) * (1.0f / 255.0f);
		g = (float)((iColor >> 8) & 0xFF) * (1.0f / 255.0f);
		b = (float)(iColor & 0xFF) * (1.0f / 255.0f);
		a = (float)((iColor >> 24) & 0xFF) * (1.0f / 255.0f);
	}

	void Color::Set(const ColorRGBA& rgbaColor)
	{
		r = (float)rgbaColor.r / 255.0f;
		g = (float)rgbaColor.g / 255.0f;
		b = (float)rgbaColor.b / 255.0f;
		a = (float)rgbaColor.a / 255.0f;
	}

	ColorBGRA Color::BGRA() const
	{
		uint32_t _a = (uint32_t)(a * 255.0f) & 255;
		uint32_t _r = (uint32_t)(r * 255.0f) & 255;
		uint32_t _g = (uint32_t)(g * 255.0f) & 255;
		uint32_t _b = (uint32_t)(b * 255.0f) & 255;

		ColorBGRA packed;
		packed.c = _b;
		packed.c |= _g << 8;
		packed.c |= _r << 16;
		packed.c |= _a << 24;
		return packed;
	}

	ColorRGBA Color::RGBA() const
	{
		uint32_t _a = (uint32_t)(a * 255.0f) & 255;
		uint32_t _r = (uint32_t)(r * 255.0f) & 255;
		uint32_t _g = (uint32_t)(g * 255.0f) & 255;
		uint32_t _b = (uint32_t)(b * 255.0f) & 255;

		ColorRGBA packed;

		packed.c = _r;
		packed.c |= _g << 8;
		packed.c |= _b << 16;
		packed.c |= _a << 24;

		return packed;
	}

	uint32_t Color::UIntBGRA() const
	{
		uint32_t _a = (uint32_t)(a * 255.0f) & 255;
		uint32_t _r = (uint32_t)(r * 255.0f) & 255;
		uint32_t _g = (uint32_t)(g * 255.0f) & 255;
		uint32_t _b = (uint32_t)(b * 255.0f) & 255;

		uint32_t value = _b;
		value |= _g << 8;
		value |= _r << 16;
		value |= _a << 24;
		return value;
	}

	uint32_t Color::UIntRGBA() const
	{
		uint32_t _a = (uint32_t)(a * 255.0f) & 255;
		uint32_t _r = (uint32_t)(r * 255.0f) & 255;
		uint32_t _g = (uint32_t)(g * 255.0f) & 255;
		uint32_t _b = (uint32_t)(b * 255.0f) & 255;

		uint32_t value = _r;
		value |= _g << 8;
		value |= _b << 16;
		value |= _a << 24;
		return value;
	}

	bool Color::FromString(const std::string& str)
	{
		return FromString(str.c_str());
	}

	bool Color::FromString(const char* str)
	{
		size_t elements = StringUtils::CountElements(str, ' ');
		if (elements < 3)
			return false;

		char* ptr = (char*)str;
		r = (float)strtod(ptr, &ptr);
		g = (float)strtod(ptr, &ptr);
		b = (float)strtod(ptr, &ptr);
		if (elements > 3)
			a = (float)strtod(ptr, &ptr);

		return true;
	}

	std::string Color::ToString() const
	{
		char tempBuffer[CONVERSION_BUFFER_LENGTH];
		sprintf(tempBuffer, "%g %g %g %g", r, g, b, a);
		return std::string(tempBuffer);
	}

	void Color::Clamp(const Color& min, const Color& max)
	{
		ALIMER_ASSERT(!(min.r > max.r || min.g > max.g || min.b > max.b || min.a > max.a));

		// Clamp the r value.
		r = Alimer::Clamp(r, min.r, max.r);

		// Clamp the g value.
		g = Alimer::Clamp(g, min.g, max.g);

		// Clamp the b value.
		b = Alimer::Clamp(b, min.b, max.b);

		// Clamp the a value.
		a = Alimer::Clamp(a, min.a, max.a);
	}

	Color Color::Clamp(const Color& color, const Color& min, const Color& max)
	{
		Color result;
		result.r = Alimer::Clamp(color.r, min.r, max.r);
		result.g = Alimer::Clamp(color.g, min.g, max.g);
		result.b = Alimer::Clamp(color.b, min.b, max.b);
		result.a = Alimer::Clamp(color.a, min.a, max.a);
		return result;
	}

	void Color::Clamp(const Color& color, const Color& min, const Color& max, Color* result)
	{
		ALIMER_ASSERT(result);

		// Clamp the x value.
		result->r = Alimer::Clamp(color.r, min.r, max.r);
		result->g = Alimer::Clamp(color.g, min.g, max.g);
		result->b = Alimer::Clamp(color.b, min.b, max.b);
		result->a = Alimer::Clamp(color.a, min.a, max.a);
	}

	void Color::Lerp(const Color& start, const Color& end, float amount, Color& result)
	{
		float invT = 1.0f - amount;
		result.r = start.r * invT + end.r * amount;
		result.g = start.g * invT + end.g * amount;
		result.b = start.b * invT + end.b * amount;
		result.a = start.a * invT + end.a * amount;
	}

	Color Color::Lerp(const Color& start, const Color& end, float amount)
	{
		float invT = 1.0f - amount;
		return Color(
			start.r * invT + end.r * amount,
			start.g * invT + end.g * amount,
			start.b * invT + end.b * amount,
			start.a * invT + end.a * amount
		);
	}

	void Color::SmoothStep(const Color& start, const Color& end, float amount, Color& result)
	{
		result.r = Alimer::SmoothStep(start.r, end.r, amount);
		result.g = Alimer::SmoothStep(start.g, end.g, amount);
		result.b = Alimer::SmoothStep(start.b, end.b, amount);
		result.a = Alimer::SmoothStep(start.a, end.a, amount);
	}

	Color Color::SmoothStep(const Color& start, const Color& end, float amount)
	{
		return Color(
			Alimer::SmoothStep(start.r, end.r, amount),
			Alimer::SmoothStep(start.g, end.g, amount),
			Alimer::SmoothStep(start.b, end.b, amount),
			Alimer::SmoothStep(start.a, end.a, amount)
		);
	}
}