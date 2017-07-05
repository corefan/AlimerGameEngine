/*
** Alimer - Copyright (C) Amer Koleci
**
** This file is subject to the terms and conditions defined in
** file 'LICENSE', which is part of this source code package.
*/

#include "Utils/StringUtils.h"

namespace Alimer
{
	int strcmpnocase(const char* s1, const char* s2)
	{
#if defined(_MSC_VER)
		return _strcmpi(s1, s2);
#else
		return strcasecmp(s1, s2);
#endif
	}

	String StringUtils::ToString(bool value, bool yesNo)
	{
		if (value)
			return yesNo ? "yes" : "true";

		return yesNo ? "no" : "false";
	}

	String StringUtils::ToString(char value)
	{
		String result;
		result += value;
		return result;
	}

	String StringUtils::ToString(int value)
	{
		StringStream ss;
		ss << value;
		return ss.str();
	}

	String StringUtils::ToString(unsigned int value)
	{
		StringStream ss;
		ss << value;
		return ss.str();
	}

	String StringUtils::ToString(long value)
	{
		StringStream ss;
		ss << value;
		return ss.str();
	}

	String StringUtils::ToString(unsigned long value)
	{
		StringStream ss;
		ss << value;
		return ss.str();
	}

	String StringUtils::ToString(long long value)
	{
		StringStream ss;
		ss << value;
		return ss.str();
	}

	String StringUtils::ToString(unsigned long long value)
	{
		StringStream ss;
		ss << value;
		return ss.str();
	}

	String StringUtils::ToString(float value)
	{
		StringStream ss;
		ss << value;
		return ss.str();
	}

	String StringUtils::ToString(double value)
	{
		StringStream ss;
		ss << value;
		return ss.str();
	}

	String StringUtils::ToString(long double value)
	{
		StringStream ss;
		ss << value;
		return ss.str();
	}

	String StringUtils::ToString(const wchar_t* value)
	{
		WString wideString = value;
		return String(wideString.begin(), wideString.end());
	}

	String StringUtils::ToString(const WString& value)
	{
		return String(value.begin(), value.end());
	}

	String StringUtils::ToString(const char* formatString, ...)
	{
		va_list args;
		va_start(args, formatString);
		String result = StringUtils::Format(formatString, args);
		va_end(args);
		return result;
	}

	WString StringUtils::ToWString(bool value, bool yesNo)
	{
		if (value)
			return yesNo ? L"yes" : L"true";

		return yesNo ? L"no" : L"false";
	}

	WString StringUtils::ToWString(char value)
	{
		WString result;
		result += value;
		return result;
	}

	WString StringUtils::ToWString(int value)
	{
		WStringStream ss;
		ss << value;
		return ss.str();
	}

	WString StringUtils::ToWString(unsigned int value)
	{
		WStringStream ss;
		ss << value;
		return ss.str();
	}

	WString StringUtils::ToWString(long value)
	{
		WStringStream ss;
		ss << value;
		return ss.str();
	}

	WString StringUtils::ToWString(unsigned long value)
	{
		WStringStream ss;
		ss << value;
		return ss.str();
	}

	WString StringUtils::ToWString(long long value)
	{
		WStringStream ss;
		ss << value;
		return ss.str();
	}

	WString StringUtils::ToWString(unsigned long long value)
	{
		WStringStream ss;
		ss << value;
		return ss.str();
	}

	WString StringUtils::ToWString(float value)
	{
		WStringStream ss;
		ss << value;
		return ss.str();
	}

	WString StringUtils::ToWString(double value)
	{
		WStringStream ss;
		ss << value;
		return ss.str();
	}

	WString StringUtils::ToWString(long double value)
	{
		WStringStream ss;
		ss << value;
		return ss.str();
	}

	WString StringUtils::ToWString(const char* value)
	{
		String stdString = String(value, strlen(value));
		return WString(stdString.begin(), stdString.end());
	}

	WString StringUtils::ToWString(const String& value)
	{
		return WString(value.begin(), value.end());
	}

	String StringUtils::Replace(const String& source, const String& oldValue, const String& newValue)
	{
		String result = source;
		String::size_type pos = 0;

		while (1)
		{
			pos = result.find(oldValue, pos);
			if (pos == String::npos)
				break;

			result.replace(pos, oldValue.size(), newValue);
			pos += newValue.size();
		}

		return result;
	}

	String StringUtils::Format(const char* format, ...)
	{
		va_list args;
		va_start(args, format);
		String result = StringUtils::Format(format, args);
		va_end(args);
		return result;
	}

	String StringUtils::Format(const char* formatString, va_list args)
	{
		// Declare a moderately sized buffer on the stack that should be
		// large enough to accommodate most log requests.
		int size = 1024;
		char stackBuffer[1024];
		std::vector<char> dynamicBuffer;
		char* str = stackBuffer;
		for (; ; )
		{
			// Pass one less than size to leave room for NULL terminator
			int needed = vsnprintf(str, size - 1, formatString, args);

			// NOTE: Some platforms return -1 when vsnprintf runs out of room, while others return
			// the number of characters actually needed to fill the buffer.
			if (needed >= 0 && needed < size)
			{
				// Successfully wrote buffer. Added a NULL terminator in case it wasn't written.
				str[needed] = '\0';
				va_end(args);
				break;
			}

			size = needed > 0 ? (needed + 1) : (size * 2);
			dynamicBuffer.resize(size);
			str = &dynamicBuffer[0];
		}

		return String(str);
	}

	int StringUtils::Compare(const char* str1, const char* str2, bool caseSensitive)
	{
		if (caseSensitive)
		{
			return strcmp(str1, str2);
		}

		return strcmpnocase(str1, str2);
	}

	int StringUtils::Compare(const String& str1, const String& str2, bool caseSensitive)
	{
		if (caseSensitive)
		{
			return strcmp(str1.c_str(), str2.c_str());
		}

		return strcmpnocase(str1.c_str(), str2.c_str());
	}

	size_t StringUtils::CountElements(const char* str, char separator)
	{
		if (!str)
			return 0;

		const char* endPos = str + strlen(str);
		const char* pos = str;
		size_t ret = 0;

		while (pos < endPos)
		{
			if (*pos != separator)
				break;
			++pos;
		}

		while (pos < endPos)
		{
			const char* start = pos;

			while (start < endPos)
			{
				if (*start == separator)
					break;

				++start;
			}

			if (start == endPos)
			{
				++ret;
				break;
			}

			const char* end = start;

			while (end < endPos)
			{
				if (*end != separator)
					break;

				++end;
			}

			++ret;
			pos = end;
		}

		return ret;
	}

	String StringUtils::ToUpper(const String& str)
	{
		String result(str);
		std::transform(result.begin(), result.end(), result.begin(), std::toupper);
		return result;
	}

	String StringUtils::ToLower(const String& str)
	{
		String result(str);
		std::transform(result.begin(), result.end(), result.begin(), std::tolower);
		return result;
	}

	String StringUtils::LeftTrim(String str)
	{
		str.erase(str.begin(), std::find_if(str.begin(), str.end(), std::not1(std::ptr_fun<int, int>(std::isspace))));
		return str;
	}

	String StringUtils::RightTrim(String str)
	{
		str.erase(std::find_if(str.rbegin(), str.rend(), std::not1(std::ptr_fun<int, int>(std::isspace))).base(), str.end());
		return str;
	}

	String StringUtils::Trim(String str)
	{
		return LeftTrim(RightTrim(str));
	}

	bool StringUtils::ParseBool(const String& str)
	{
		if (str.empty())
			return false;

		size_t length = str.length();

		for (size_t i = 0; i < length; ++i)
		{
			char c = Alimer::ToLower(str[i]);
			if (c == 't' || c == 'y' || c == '1')
				return true;
			else if (c != ' ' && c != '\t')
				break;
		}

		return false;
	}

	int32_t StringUtils::ParseInt(const String& source)
	{
		if (source.empty())
			return 0;

		// Explicitly ask for base 10 to prevent source starts with '0' or '0x' from being converted to base 8 or base 16, respectively
		return strtol(source.c_str(), 0, 10);
	}

	uint32_t StringUtils::ParseUInt(const String& source)
	{
		if (source.empty())
			return 0;

		return strtoul(source.c_str(), 0, 10);
	}

	float StringUtils::ParseFloat(const String& source)
	{
		if (source.empty())
			return 0.0f;

		return (float)strtod(source.c_str(), 0);
	}

	double StringUtils::ParseDouble(const String& source)
	{
		if (source.empty())
			return 0.0;

		return strtod(source.c_str(), 0);
	}


	bool StringUtils::ParseVector2(const String& str, Vector2* result)
	{
		ALIMER_ASSERT(result);

		size_t elements = StringUtils::CountElements(str.c_str(), ' ');
		if (elements < 2)
		{
			result->Set(0.0f, 0.0f);
			return false;
		}

		char* ptr = (char*)str.data();
		result->x = (float)strtod(ptr, &ptr);
		result->y = (float)strtod(ptr, &ptr);

		return true;
	}
}