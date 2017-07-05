/*
** Alimer - Copyright (C) Amer Koleci
**
** This file is subject to the terms and conditions defined in
** file 'LICENSE', which is part of this source code package.
*/

#pragma once

#include "Prerequisites.h"
#include "Math/Vector2.h"

namespace Alimer
{
	/// Convert a char to uppercase.
	ALIMER_FORCE_INLINE char ToUpper(char c) { return (c >= 'a' && c <= 'z') ? c - 0x20 : c; }
	/// Convert a char to lowercase.
	ALIMER_FORCE_INLINE char ToLower(char c) { return (c >= 'A' && c <= 'Z') ? c + 0x20 : c; }
	/// Return whether a char is an alphabet letter.
	ALIMER_FORCE_INLINE bool IsAlpha(char c) { return (c >= 'a' && c <= 'z') || (c >= 'A' && c <= 'Z'); }
	/// Return whether a char is a digit.
	ALIMER_FORCE_INLINE bool IsDigit(char c) { return c >= '0' && c <= '9'; }

	/**
	* String utils class.
	*/
	class ALIMER_API StringUtils
	{
	public:
		static String ToString(bool value, bool yesNo = false);
		static String ToString(char value);
		static String ToString(int value);
		static String ToString(unsigned int value);
		static String ToString(long value);
		static String ToString(unsigned long value);
		static String ToString(long long value);
		static String ToString(unsigned long long value);
		static String ToString(float value);
		static String ToString(double value);
		static String ToString(long double value);
		static String ToString(const wchar_t* value);
		static String ToString(const WString& value);
		/// Return a formatted string.
		static String ToString(const char* formatString, ...);

		static WString ToWString(bool value, bool yesNo = false);
		static WString ToWString(char value);
		static WString ToWString(int value);
		static WString ToWString(unsigned int value);
		static WString ToWString(long value);
		static WString ToWString(unsigned long value);
		static WString ToWString(long long value);
		static WString ToWString(unsigned long long value);
		static WString ToWString(float value);
		static WString ToWString(double value);
		static WString ToWString(long double value);
		static WString ToWString(const char* value);
		static WString ToWString(const String& value);

		static String Replace(const String& source, const String& oldValue, const String& newValue);

		static String Format(const char* format, ...);
		static String Format(const char* format, va_list args);

		/// Compare two C strings.
		static int Compare(const char* str1, const char* str2, bool caseSensitive);

		/// Compare two C strings.
		static int Compare(const String& str1, const String& str2, bool caseSensitive);

		/// Return the amount of substrings split by a separator char.
		static size_t CountElements(const char* str, char separator);

		static String ToUpper(const String& str);
		static String ToLower(const String& str);

		static String LeftTrim(String str);
		static String RightTrim(String str);
		static String Trim(String str);

		/// Parse a bool from a C string. Check for the first non-empty character (converted to lowercase) being either 't', 'y' or '1'.
		static bool ParseBool(const String& source);
		/// Parse an integer from a C string.
		static int32_t ParseInt(const String& source);
		/// Parse an unsigned integer from a C string.
		static uint32_t ParseUInt(const String& source);
		/// Parse a float from a C string.
		static float ParseFloat(const String& source);
		/// Parse a double from a C string.
		static double ParseDouble(const String& source);

		/// Parse from a string. Return true on success.
		static bool ParseVector2(const String& str, Vector2* result);

	private:
		StringUtils() { }
		~StringUtils() { }

		DISALLOW_COPY_AND_ASSIGN(StringUtils);
	};

}
