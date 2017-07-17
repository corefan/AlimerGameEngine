/*
** Alimer - Copyright (C) 2016-2017 Amer Koleci.
**
** This file is subject to the terms and conditions defined in
** file 'LICENSE.md', which is part of this source code package.
*/

#pragma once

#include "Prerequisites.h"

#if ALIMER_LINUX_FAMILY
#	include <pthread.h>
#endif

namespace Alimer
{
#if ALIMER_LINUX_FAMILY
	typedef pthread_t ThreadID;
#else
	typedef unsigned ThreadID;
#endif

	/**
	* Operating system thread.
	*/
	class ALIMER_API Thread
	{
	public:

		/**
		* Destructor.
		*/
		virtual ~Thread();

		/// Set the current thread as the main thread.
		static void SetMainThread();
		/// Return the current thread's ID.
		static ThreadID GetCurrentThreadID();
		/// Return whether is executing in the main thread.
		static bool IsMainThread();

	private:

		/// Main thread's thread ID.
		static ThreadID _mainThreadID;
	};
}