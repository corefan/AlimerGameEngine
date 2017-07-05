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
	/**
	* Defines an integer rectangle.
	*/
	class ALIMER_API IntRect
	{
	public:
		/**
		* Specifies the x-coordinate of the rectangle.
		*/
		int32_t x;

		/**
		* Specifies the y-coordinate of the rectangle.
		*/
		int32_t y;

		/**
		* Specifies the width of the rectangle.
		*/
		uint32_t width;

		/**
		* Specifies the height of the rectangle.
		*/
		uint32_t height;

		/// Zero vector.
		static const IntRect EMPTY;

		/**
		* Constructs a new rectangle initialized to all zeros.
		*/
		IntRect();

		/**
		* Constructs a new rectangle with the x = 0, y = 0 and the specified width and height.
		*
		* @param width_ The width of the rectangle.
		* @param height_ The height of the rectangle.
		*/
		IntRect(uint32_t width_, uint32_t height_);

		/**
		* Constructs a new rectangle with the specified x, y, width and height.
		*
		* @param x_ The x-coordinate of the rectangle.
		* @param y_ The y-coordinate of the rectangle.
		* @param width_ The width of the rectangle.
		* @param height_ The height of the rectangle.
		*/
		IntRect(int x_, int y_, uint32_t width_, uint32_t height_);

		/**
		* Constructs a new rectangle that is a copy of the specified rectangle.
		*
		* @param other The rectangle to copy.
		*/
		IntRect(const IntRect& other);

		/**
		* Destructor.
		*/
		~IntRect();

		/**
		* Gets a value that indicates whether the rectangle is empty.
		*
		* @return true if the rectangle is empty, false otherwise.
		*/
		bool IsEmpty() const;

		/**
		* Returns the x-coordinate of the right side of the rectangle.
		*
		* @return The x-coordinate of the right side of the rectangle.
		*/
		int32_t GetRight() const {
			return x + width;
		}

		/**
		* Returns the y-coordinate of the bottom of the rectangle.
		*
		* @return The y-coordinate of the bottom of the rectangle.
		*/
		int32_t GetBottom() const {
			return y + height;
		}

		/// Return as string.
		String ToString() const;

		/// Return hash value.
		uint32_t ToHash() const;

		/// Comparision operators
		bool operator == (const IntRect& other) const;
		bool operator != (const IntRect& other) const;

		/// Assignment operators
		IntRect& operator= (const IntRect& value);
	};

	inline bool IntRect::operator == (const IntRect& other) const
	{
		return
			x == other.x &&
			y == other.y &&
			width == other.width &&
			height == other.height;
	}

	inline bool IntRect::operator != (const IntRect& other) const
	{
		return
			x != other.x &&
			y != other.y &&
			width != other.width &&
			height != other.height;
	}

	inline IntRect& IntRect::operator = (const IntRect& value)
	{
		this->x = value.x;
		this->y = value.y;
		this->width = value.width;
		this->height = value.height;

		return *this;
	}
}
