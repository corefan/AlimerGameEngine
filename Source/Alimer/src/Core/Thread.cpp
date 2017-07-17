/*
** Alimer - Copyright (C) 2016-2017 Amer Koleci.
**
** This file is subject to the terms and conditions defined in
** file 'LICENSE.md', which is part of this source code package.
*/

#include "Core/Thread.h"

#if ALIMER_WINDOWS_FAMILY
#	include <windows.h>
#else
#	include <unistd.h>
#endif

namespace Alimer
{
	ThreadID Thread::_mainThreadID = Thread::GetCurrentThreadID();

	Thread::~Thread()
	{
	}

	void Thread::SetMainThread()
	{
		_mainThreadID = GetCurrentThreadID();
	}

	ThreadID Thread::GetCurrentThreadID()
	{
#ifdef _WIN32
		return GetCurrentThreadId();
#else
		return pthread_self();
#endif
	}

	bool Thread::IsMainThread()
	{
		return GetCurrentThreadID() == _mainThreadID;
	}
}