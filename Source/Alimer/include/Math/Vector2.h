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
	/**
	* Defines a vector with two floating coordinates.
	*/
	class ALIMER_API Vector2
	{
	public:
		/**
		* The x-coordinate of the vector.
		*/
		float x;

		/**
		* The y-coordinate of the vector.
		*/
		float y;

		/// Zero vector.
		static const Vector2 ZERO;
		/// (-1,0) vector.
		static const Vector2 LEFT;
		/// (1,0) vector.
		static const Vector2 RIGHT;
		/// (0,1) vector.
		static const Vector2 UP;
		/// (0,-1) vector.
		static const Vector2 DOWN;
		/// (1,1) vector.
		static const Vector2 ONE;

		/// (1,0) vector.
		static const Vector2 UNIT_X;

		/// (0,1) vector.
		static const Vector2 UNIT_Y;

		/**
		* Constructs a new vector initialized to all zeros.
		*/
		Vector2();

		/**
		* Constructor.
		*
		* @param value Value to initialize both components to.
		*/
		Vector2(float value);

		/**
		* Constructor.
		*
		* @param x_ The x coordinate.
		* @param y_ The y coordinate.
		*/
		Vector2(float x_, float y_);

		/**
		* Constructor.
		*
		* @param floatArray An array containing the elements of the vector in the order x, y.
		*/
		Vector2(const float* floatArray);

		/**
		* Constructor.
		*
		* @param copy The vector to copy.
		*/
		Vector2(const Vector2& copy);

		/**
		* Destructor.
		*/
		~Vector2();

		/**
		* Indicates whether this vector contains all zeros.
		*
		* @return true if this vector contains all zeros, false otherwise.
		*/
		bool IsZero() const;

		/**
		* Indicates whether this vector contains all ones.
		*
		* @return true if this vector contains all ones, false otherwise.
		*/
		bool IsOne() const;

		/**
		* Sets the elements of this vector to the specified values.
		*
		* @param x_ The new x coordinate.
		* @param y_ The new y coordinate.
		*/
		void Set(float x_, float y_);

		/**
		* Sets the elements of this vector from the values in the specified array.
		*
		* @param floatArray An array containing the elements of the vector in the order x, y.
		*/
		void Set(const float* floatArray);

		/**
		* Sets the elements of this vector to those in the specified vector.
		*
		* @param other The vector to copy.
		*/
		void Set(const Vector2& other);

		/**
		* Calculates the length of the vector.
		*/
		float Length() const;

		/**
		* Calculates the length of the vector squared.
		*/
		float LengthSquared() const;

		/**
		* Calculates the distance between two vectors..
		*
		* @param v1 The first vector.
		* @param v2 The second vector.
		*
		* @return The distance between the vectors.
		*/
		static float Distance(const Vector2& v1, const Vector2& v2);

		/**
		* Calculated the squared distance between the specified vectors.
		*
		* @param v1 The first vector.
		* @param v2 The second vector.
		*
		* @return The squared distance between the vectors.
		*/
		static float DistanceSquared(const Vector2& v1, const Vector2& v2);

		/**
		* Calculates the dot product between this vector and another vector.
		*
		* @param vector The vector to compute the dot product with.
		* @return The dot product.
		*/
		float Dot(const Vector2& vector) const;

		/**
		* Calculates the dot product of two vectors.
		*
		* @param v1 The first vector.
		* @param v2 The second vector.
		* @return The dot product between the vectors.
		*/
		static float Dot(const Vector2& v1, const Vector2& v2);

		/**
		* Performs a linear interpolation between two vectors.
		*
		* @param left The start vector.
		* @param end The end vector.
		* @param amount The Value between 0 and 1 indicating the weight of end vector.
		* @param result A Vector to store the result in.
		*/
		static void Lerp(const Vector2& start, const Vector2& end, float amount, Vector2* result);

		/**
		* Performs a linear interpolation between two vectors.
		*
		* @param left The start vector.
		* @param end The end vector.
		* @param amount The Value between 0 and 1 indicating the weight of end vector.
		* @return A Vector to store the result in.
		*/
		static Vector2 Lerp(const Vector2& start, const Vector2& end, float amount);

		/**
		* Return float data.
		*/
		const float* Data() const { return &x; }

		/// Return as string.
		String ToString() const;

		/// Return hash value.
		uint32_t ToHash() const;

		/// Comparision operators
		bool operator == (const Vector2& other) const;
		bool operator != (const Vector2& other) const;

		/// Assignment operators
		Vector2& operator= (const Vector2& value);
		Vector2& operator+= (const Vector2& value);
		Vector2& operator-= (const Vector2& value);
		Vector2& operator*= (const Vector2& value);
		Vector2& operator*= (float value);
		Vector2& operator/= (float value);
		Vector2& operator/= (const Vector2& value);

		/// Add a vector.
		Vector2 operator + (const Vector2& value) const;

		/// Return negation.
		Vector2 operator- () const;

		/// Subtract a vector.
		Vector2 operator - (const Vector2& value) const;

		/// Multiply with a scalar.
		Vector2 operator * (float value) const;

		/// Multiply with a vector.
		Vector2 operator * (const Vector2& value) const;

		/// Divide by a scalar.
		Vector2 operator / (float value) const;
		/// Divide by a vector.
		Vector2 operator / (const Vector2& value) const;
	};

	inline bool Vector2::operator == (const Vector2& other) const
	{
		return
			x == other.x &&
			y == other.y;
	}

	inline bool Vector2::operator != (const Vector2& other) const
	{
		return
			x != other.x &&
			y != other.y;
	}

	inline Vector2& Vector2::operator = (const Vector2& v)
	{
		this->x = v.x;
		this->y = v.y;

		return *this;
	}


	inline Vector2& Vector2::operator+= (const Vector2& value)
	{
		x += value.x;
		y += value.y;

		return *this;
	}

	inline Vector2& Vector2::operator-= (const Vector2& value)
	{
		x -= value.x;
		y -= value.y;

		return *this;
	}

	inline Vector2& Vector2::operator*= (const Vector2& value)
	{
		x *= value.x;
		y *= value.y;

		return *this;
	}

	inline Vector2& Vector2::operator*= (float value)
	{
		x *= value;
		y *= value;

		return *this;
	}

	inline Vector2& Vector2::operator/= (float value)
	{
		ALIMER_ASSERT(value != 0.0f);

		float fInv = 1.0f / value;

		x *= fInv;
		y *= fInv;

		return *this;
	}

	inline Vector2& Vector2::operator/= (const Vector2& value)
	{
		x /= value.x;
		y /= value.y;

		return *this;
	}

	inline Vector2 Vector2::operator- () const { return Vector2(-x, -y); }

	//------------------------------------------------------------------------------
	// Binary operators
	//------------------------------------------------------------------------------
	inline Vector2 Vector2::operator + (const Vector2& value) const { return Vector2(x + value.x, y + value.y); }
	inline Vector2 Vector2::operator - (const Vector2& value) const { return Vector2(x - value.x, y - value.y); }
	inline Vector2 Vector2::operator * (float value) const { return Vector2(x * value, y * value); }
	inline Vector2 Vector2::operator * (const Vector2& value) const { return Vector2(x * value.x, y * value.y); }
	inline Vector2 Vector2::operator / (float value) const { return Vector2(x / value, y / value); }
	inline Vector2 Vector2::operator / (const Vector2& value) const { return Vector2(x / value.x, y / value.y); }

	// Binary operators
	inline Vector2 operator * (float scalar, const Vector2& vector) { return vector * scalar; }
}
