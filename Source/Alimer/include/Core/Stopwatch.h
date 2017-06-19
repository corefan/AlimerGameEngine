/*
** Alimer - Copyright (C) 2016-2017 Amer Koleci.
**
** This file is subject to the terms and conditions defined in
** file 'LICENSE.md', which is part of this source code package.
*/

#pragma once

#include "Prerequisites.h"
#include "Core/TimeSpan.h"

namespace Alimer
{
	/**
	* Operating system stopwatch.
	*/
	class ALIMER_API Stopwatch
	{
	public:
		/**
		* Constructor.
		*/
		Stopwatch();

		/**
		* Destructor.
		*/
		virtual ~Stopwatch();

		/**
		* Resets the Stopwatch
		*/
		void Reset();

		/**
		* Starts the Stopwatch
		*/
		void Start();

		/**
		* Stops the Stopwatch.
		*/
		void Stop();

		/**
		* Restarts the Stopwatch.
		*/
		void Restart();

		/// Is stopwatch running?
		bool IsRunning() const {
			return _isRunning;
		}

		uint64_t GetElapsedMilliseconds() const;
		uint64_t GetElapsedTicks() const;
		TimeSpan GetElapsed() const;

		static bool IsHighResolution();
		static uint64_t GetFrequency();
		static uint64_t GetTimestamp();

		/// Sleeps the current thread for the given amount of milliseconds.
		static void Sleep(uint32_t milliseconds);

	private:
		uint64_t GetRawElapsedTicks() const;
		uint64_t GetElapsedDateTimeTicks() const;

		uint64_t _elapsed;
		uint64_t _startTimeStamp;
		bool _isRunning;
	};
}