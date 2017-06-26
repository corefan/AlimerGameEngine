/*
** Alimer - Copyright (C) Amer Koleci
**
** This file is subject to the terms and conditions defined in
** file 'LICENSE.md', which is part of this source code package.
*/

#include "Utils/StringUtils.h"

namespace Alimer
{
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
		int pos = 0, lastPos = 0;
		int length = (int)strlen(formatString);

		String result;

		while (true)
		{
			// Scan the format string and find %a argument where a is one of d, f, s ...
			while (pos < length && formatString[pos] != '%') pos++;
			{
				result.append(formatString + lastPos, pos - lastPos);
			}

			if (pos >= length)
			{
				return result;
			}

			char format = formatString[pos + 1];
			pos += 2;
			lastPos = pos;

			switch (format)
			{
				// Integer
			case 'd':
			case 'i':
			{
				int arg = va_arg(args, int);
				result.append(ToString(arg));
				break;
			}

			// Unsigned
			case 'u':
			{
				unsigned arg = va_arg(args, unsigned);
				result.append(ToString(arg));
				break;
			}

			// Unsigned long
			case 'l':
			{
				unsigned long arg = va_arg(args, unsigned long);
				result.append(ToString(arg));
				break;
			}

			// Real
			case 'f':
			{
				double arg = va_arg(args, double);
				result.append(ToString(arg));
				break;
			}

			// Character
			case 'c':
			{
				int arg = va_arg(args, int);
				result.append(ToString(arg));
				break;
			}

			// C string
			case 's':
			{
				char* arg = va_arg(args, char*);
				result.append(arg);
				break;
			}

			// Hex
			case 'x':
			{
				char buf[CONVERSION_BUFFER_LENGTH];
				int arg = va_arg(args, int);
				int arglen = ::sprintf(buf, "%x", arg);
				result.append(buf, (size_t)arglen);
				break;
			}

			// Pointer
			case 'p':
			{
				char buf[CONVERSION_BUFFER_LENGTH];
				int arg = va_arg(args, int);
				int arglen = ::sprintf(buf, "%p", reinterpret_cast<void*>(arg));
				result.append(buf, (size_t)arglen);
				break;
			}

			case '%':
			{
				result.append("%", 1);
				break;
			}

			default:
				ALIMER_LOGWARNING("Unsupported format specifier: '%c'", format);
				break;
			}
		}

		return result;
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