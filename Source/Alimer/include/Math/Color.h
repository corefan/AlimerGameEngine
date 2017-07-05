/*
** Alimer - Copyright (C) Amer Koleci
**
** This file is subject to the terms and conditions defined in
** file 'LICENSE', which is part of this source code package.
*/

#pragma once

#include "Prerequisites.h"

namespace Alimer
{
	// ARGB Color; 8-8-8-8 bit unsigned normalized integer components packed into
	// a 32 bit integer.
	struct ALIMER_API ColorBGRA
	{
		union
		{
			struct
			{
				uint8_t b;  // Blue:    0/255 to 255/255
				uint8_t g;  // Green:   0/255 to 255/255
				uint8_t r;  // Red:     0/255 to 255/255
				uint8_t a;  // Alpha:   0/255 to 255/255
			};
			uint32_t c;
		};
	};

	// 4D Vector; 8 bit unsigned normalized integer components
	struct ALIMER_API ColorRGBA
	{
		union
		{
			struct
			{
				uint8_t r;
				uint8_t g;
				uint8_t b;
				uint8_t a;
			};
			uint32_t c;
		};
	};

	/**
	* Defines a color in the form of rgba (float value).
	*/
	class ALIMER_API Color
	{
	public:
		/**
		* The red component of the color.
		*/
		float r;

		/**
		* The green component of the color.
		*/
		float g;

		/**
		* The blue component of the color.
		*/
		float b;

		/**
		* The alpha component of the color.
		*/
		float a;

		/**
		* Constructs a new vector initialized to all zeros.
		*/
		Color();

		/**
		* Constructs a new color initialized to the specified values and alpha to 1.0f.
		*
		* @param r_ The red value.
		* @param g_ The green value.
		* @param b_ The blue value.
		*/
		Color(float r_, float g_, float b_);

		/**
		* Constructs a new color initialized to the specified values.
		*
		* @param r_ The red value.
		* @param g_ The green value.
		* @param b_ The blue value.
		* @param a_ The alpha value.
		*/
		Color(float r_, float g_, float b_, float a_);

		/**
		* Constructs a new color from the values in the specified array.
		*
		* @param floatArray An array containing the elements of the vector in the order r, g, b, a.
		*/
		Color(const float* floatArray);

		/**
		* Constructor.
		*
		* Creates a new color that is a copy of the specified color.
		*
		* @param copy The color to copy.
		*/
		Color(const Color& copy);

		/**
		* Constructor.
		*
		* Creates a new color that is a copy of the specified color and different alpha.
		*
		* @param color The color to get r, g, b from.
		* @param alpha The alpha value to set.
		*/
		Color(const Color& color, float alpha);

		/**
		* Constructor.
		*
		* Creates a new color from packed bgra color.
		*
		* @param bgraColor The packed color.
		*/
		explicit Color(const ColorBGRA& bgraColor);

		/**
		* Constructor.
		*
		* Creates a new color from packed rgba color.
		*
		* @param bgraColor The packed color.
		*/
		explicit Color(const ColorRGBA& rgbaColor);

		/**
		* Destructor.
		*/
		~Color();

		/**
		* Sets the elements of this color to the specified values.
		*
		* @param r_ The red value.
		* @param g_ The green value.
		* @param b_ The blue value.
		* @param a_ The alpha value.
		*/
		void Set(float r_, float g_, float b_, float a_);

		/**
		* Sets the elements of this color from the values in the specified array.
		*
		* @param floatArray An array containing the elements of the color in the order r, g, b, a.
		*/
		void Set(const float* floatArray);

		/**
		* Sets the elements of this color to those in the specified color.
		*
		* @param copy The color to copy.
		*/
		void Set(const Color& copy);

		/**
		* Sets the elements of this color to those in the specified packed color.
		*
		* @param copy The packed color to copy.
		*/
		void Set(const ColorBGRA& copy);

		/**
		* Sets the elements of this color to those in the specified packed color.
		*
		* @param copy The packed color to copy.
		*/
		void Set(const ColorRGBA& copy);

		/// Multiply with a scalar.
		Color operator *(float scalar) const { return Color(r * scalar, g * scalar, b * scalar, a * scalar); }

		/// Add a color.
		Color operator +(const Color& other) const { return Color(r + other.r, g + other.g, b + other.b, a + other.a); }

		/// Return negation.
		Color operator -() const { return Color(-r, -g, -b, -a); }

		/// Substract a color.
		Color operator -(const Color& other) const { return Color(r - other.r, g - other.g, b - other.b, a - other.a); }

		/// Add-assign a color.
		Color& operator +=(const Color& other)
		{
			r += other.r;
			g += other.g;
			b += other.b;
			a += other.a;
			return *this;
		}

		/// Return float data.
		const float* Data() const { return &r; }

		/// Parse from a string. Return true on success.
		bool FromString(const std::string& str);
		/// Parse from a C string. Return true on success.
		bool FromString(const char* str);

		/// Return as string.
		std::string ToString() const;

		ColorBGRA BGRA() const;
		ColorRGBA RGBA() const;

		uint32_t UIntBGRA() const;
		uint32_t UIntRGBA() const;

		/**
		* Clamps this color within the specified range.
		*
		* @param min The minimum value.
		* @param max The maximum value.
		*/
		void Clamp(const Color& min, const Color& max);

		/**
		* Clamps the specified color within the specified range and returns it in dst.
		*
		* @param color The color to clamp.
		* @param min The minimum value.
		* @param max The maximum value.
		*
		* @return The clamped color.
		*/
		static Color Clamp(const Color& color, const Color& min, const Color& max);

		/**
		* Clamps the specified color within the specified range and returns it in dst.
		*
		* @param color The color to clamp.
		* @param min The minimum value.
		* @param max The maximum value.
		* @param result A color to store the result in.
		*/
		static void Clamp(const Color& color, const Color& min, const Color& max, Color* result);

		/**
		* Performs a linear interpolation between two colors.
		*
		* @param start Start color.
		* @param end End color.
		* @param amount Value between 0 and 1 indicating the weight of end.
		* @param result The destination color.
		*/
		static void Lerp(const Color& start, const Color& end, float amount, Color& result);

		/**
		* Performs a linear interpolation between two colors.
		*
		* @param start Start color.
		* @param end End color.
		* @param amount Value between 0 and 1 indicating the weight of end.
		*
		* @return The linear interpolation of the two colors.
		*/
		static Color Lerp(const Color& start, const Color& end, float amount);

		/**
		* Performs a cubic interpolation between two colors.
		*
		* @param start Start color.
		* @param end End color.
		* @param amount Value between 0 and 1 indicating the weight of end.
		* @param result The destination color.
		*/
		static void SmoothStep(const Color& start, const Color& end, float amount, Color& result);

		/**
		* Performs a cubic interpolation between two colors.
		*
		* @param start Start color.
		* @param end End color.
		* @param amount Value between 0 and 1 indicating the weight of end.
		*
		* @return The cubic  interpolation of the two colors.
		*/
		static Color SmoothStep(const Color& start, const Color& end, float amount);

		/**
		* operator =
		*/
		inline Color& operator = (const Color& other) {
			r = other.r;
			g = other.g;
			b = other.b;
			a = other.a;
			return *this;
		}

		// Comparision operators
		inline bool operator == (const Color& other) const {
			return
				r == other.r &&
				g == other.g &&
				b == other.b &&
				a == other.a;
		}

		inline bool operator != (const Color& other) const
		{
			return
				r != other.r &&
				g != other.g &&
				b != other.b &&
				a != other.a;
		}
	};

	/// Multiply Color with a scalar.
	inline Color operator *(float scalar, const Color& right) { return right * scalar; }
}