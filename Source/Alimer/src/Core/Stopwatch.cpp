/*
** Alimer - Copyright (C) 2016-2017 Amer Koleci.
**
** This file is subject to the terms and conditions defined in
** file 'LICENSE.md', which is part of this source code package.
*/

#include "Core/Stopwatch.h"

#if defined(__unix__) || defined(__linux__) || defined(__EMSCRIPTEN__) || defined(__ANDROID__) || defined(__APPLE__) || defined (__CYGWIN__)
#include <time.h>
#include <errno.h>
#include <string.h>
#include <sys/time.h>
#endif

#ifdef _WIN32
#include <windows.h>
#endif

#ifdef __EMSCRIPTEN__
#include <emscripten/emscripten.h>
#endif

namespace Alimer
{
	static bool s_isHighResolution = true;
	static double s_tickFrequency = 0.0;

#ifdef _WIN32
	static LARGE_INTEGER ddwTimerFrequency;
#endif


	struct PlatformStopwatchInitializer
	{
		PlatformStopwatchInitializer()
		{
#ifdef _WIN32
			if (!QueryPerformanceFrequency(&ddwTimerFrequency))
			{
				s_isHighResolution = false;
			}

#endif

			if (s_isHighResolution)
			{
				s_tickFrequency = TimeSpan::TicksPerSecond;
				s_tickFrequency /= Stopwatch::GetFrequency();
			}
			else
			{
				s_tickFrequency = 1.0;
			}
		}
	};

	PlatformStopwatchInitializer __platformStopwatchInitializer__;

	Stopwatch::Stopwatch()
		: _elapsed(0)
		, _startTimeStamp(0)
		, _isRunning(false)
	{
		Reset();
	}

	Stopwatch::~Stopwatch()
	{
		Reset();
	}

	void Stopwatch::Reset()
	{
		_elapsed = 0;
		_startTimeStamp = 0;
		_isRunning = false;
	}

	void Stopwatch::Start()
	{
		// Calling start on a running Stopwatch is a no-op. 
		if (!_isRunning)
		{
			_startTimeStamp = Stopwatch::GetTimestamp();
			_isRunning = true;
		}
	}

	void Stopwatch::Stop()
	{
		// Calling stop on a stopped Stopwatch is a no-op.
		if (_isRunning)
		{
			int64_t endTimeStamp = Stopwatch::GetTimestamp();
			int64_t elapsedThisPeriod = endTimeStamp - _startTimeStamp;
			_elapsed += elapsedThisPeriod;
			_isRunning = false;

			if (_elapsed < 0)
			{
				// When measuring small time periods the StopWatch.Elapsed* 
				// properties can return negative values.  This is due to 
				// bugs in the basic input/output system (BIOS) or the hardware
				// abstraction layer (HAL) on machines with variable-speed CPUs
				// (e.g. Intel SpeedStep).

				_elapsed = 0;
			}
		}
	}

	void Stopwatch::Restart()
	{
		_elapsed = 0;
		_startTimeStamp = Stopwatch::GetTimestamp();
		_isRunning = true;
	}

	uint64_t Stopwatch::GetElapsedMilliseconds() const
	{
		return GetElapsedDateTimeTicks() / (uint64_t)TimeSpan::TicksPerMillisecond;
	}

	uint64_t Stopwatch::GetElapsedTicks() const
	{
		return GetRawElapsedTicks();
	}

	uint64_t Stopwatch::GetRawElapsedTicks() const
	{
		uint64_t timeElapsed = _elapsed;

		if (_isRunning)
		{
			// If the StopWatch is running, add elapsed time since
			// the Stopwatch is started last time. 
			uint64_t currentTimeStamp = GetTimestamp();
			uint64_t elapsedUntilNow = currentTimeStamp - _startTimeStamp;
			timeElapsed += elapsedUntilNow;
		}

		return timeElapsed;
	}

	uint64_t Stopwatch::GetElapsedDateTimeTicks() const
	{
		uint64_t rawTicks = GetRawElapsedTicks();
		if (s_isHighResolution)
		{
			// convert high resolution perf counter to DateTime ticks
			double dticks = double(rawTicks);
			dticks *= s_tickFrequency;
			return (uint64_t)dticks;
		}
		else
		{
			return rawTicks;
		}
	}

	TimeSpan Stopwatch::GetElapsed() const
	{
		return TimeSpan(GetElapsedDateTimeTicks());
	}

	bool Stopwatch::IsHighResolution()
	{
		return s_isHighResolution;
	}

	uint64_t Stopwatch::GetFrequency()
	{
#if defined(__ANDROID__)
		return 1000000000ULL; // 1e9 == nanoseconds.
#elif defined(__EMSCRIPTEN__)
		return 1000000ULL; // 1e6 == microseconds.
		//	return CLOCKS_PER_SEC;
#elif defined(_WIN32)
		return ddwTimerFrequency.QuadPart;
#elif defined(_POSIX_MONOTONIC_CLOCK)
		return 1000 * 1000 * 1000;
#elif defined(_POSIX_C_SOURCE) || defined(__APPLE__)
		return 1000 * 1000;
#else
		return CLOCKS_PER_SEC;
#endif
	}

	uint64_t Stopwatch::GetTimestamp()
	{
#if defined(__ANDROID__)
		struct timespec res;
		clock_gettime(CLOCK_REALTIME, &res);
		return 1000000000ULL * res.tv_sec + (uint64_t)res.tv_nsec;
#elif defined(__EMSCRIPTEN__)
		// emscripten_get_now() returns a wallclock time as a float in milliseconds (1e-3).
		// scale it to microseconds (1e-6) and return as a tick.
		return (uint64_t)(((double)emscripten_get_now()) * 1e3);
		//	return (uint64_t)clock();
#elif defined(_WIN32)
		LARGE_INTEGER ddwTimer;
		QueryPerformanceCounter(&ddwTimer);
		return ddwTimer.QuadPart;
#elif defined(_POSIX_MONOTONIC_CLOCK)
		timespec t;
		clock_gettime(CLOCK_MONOTONIC, &t);
		return (uint64_t)t.tv_sec * 1000 * 1000 * 1000 + (uint64_t)t.tv_nsec;
#elif defined(_POSIX_C_SOURCE) || defined(__APPLE__)
		timeval t;
		gettimeofday(&t, NULL);
		return (uint64_t)t.tv_sec * 1000 * 1000 + (uint64_t)t.tv_usec;
#else
		return (uint64_t)clock();
#endif
	}

	void Stopwatch::Sleep(uint32_t milliseconds)
	{
#ifdef _WIN32
		::Sleep(milliseconds);
#else
		timespec time;
		time.tv_sec = milliseconds / 1000;
		time.tv_nsec = (milliseconds % 1000) * 1000000;
		int ret = nanosleep(&time, 0);
		if (ret == -1)
			ALIMER_LOGERROR("nanosleep returned -1! Reason: %s(%d).", strerror(errno), (int)errno);
#endif
	}
}