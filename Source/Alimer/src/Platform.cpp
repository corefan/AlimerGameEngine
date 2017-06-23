/*
** Alimer - Copyright (C) 2016-2017 Amer Koleci.
**
** This file is subject to the terms and conditions defined in
** file 'LICENSE.md', which is part of this source code package.
*/

#include "Prerequisites.h"
#include <cctype>
#include <cstdarg>

#if defined(ALIMER_WINDOWS)
#	include <windows.h>
#elif defined(ALIMER_ANDROID)
#	include <android/log.h>
#endif

namespace Alimer
{
	void Platform::Print(const char* format, ...)
	{
		ALIMER_ASSERT(format);
		va_list argptr;
		va_start(argptr, format);

#ifdef ALIMER_WINDOWS
		int sz = vfprintf(stderr, format, argptr);
		if (sz > 0)
		{
			char* buffer(new char[sz + 1]);
			vsprintf(buffer, format, argptr);
			buffer[sz] = 0;
			OutputDebugStringA(buffer);
			SafeDeleteArray(buffer);
		}
#elif ALIMER_ANDROID
		__android_log_vprint(ANDROID_LOG_INFO, "alimer-native-activity", format, argptr);
#else
		vfprintf(stderr, format, argptr);
#endif

		va_end(argptr);
	}

}