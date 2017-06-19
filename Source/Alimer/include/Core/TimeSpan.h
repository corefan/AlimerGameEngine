/*
** Alimer - Copyright (C) Amer Koleci
**
** This file is subject to the terms and conditions defined in
** file 'LICENSE.txt', which is part of this source code package.
*/

#pragma once

#include "Prerequisites.h"

namespace Alimer
{
	/**
	* TimeSpan class.
	*/
	class ALIMER_API TimeSpan
	{
	public:
		static const int64_t TicksPerMillisecond = 10000;
		static const int64_t TicksPerSecond = TicksPerMillisecond * 1000;	// 10,000,000
		static const int64_t TicksPerMinute = TicksPerSecond * 60;			// 600,000,000
		static const int64_t TicksPerHour = TicksPerMinute * 60;			// 36,000,000,000
		static const int64_t TicksPerDay = TicksPerHour * 24;				// 864,000,000,000

		static const TimeSpan Zero;
		static const TimeSpan MaxValue;
		static const TimeSpan MinValue;

		/**
		* Constructor.
		*/
		TimeSpan();

		/**
		* Constructor.
		*/
		TimeSpan(int64_t ticks_);

		/**
		* Constructor.
		*/
		TimeSpan(uint32_t hours, uint32_t minutes, uint32_t seconds);

		/**
		* Constructor.
		*/
		TimeSpan(uint32_t days, uint32_t hours, uint32_t minutes, uint32_t seconds);

		/**
		* Constructor.
		*/
		TimeSpan(uint32_t days, uint32_t hours, uint32_t minutes, uint32_t seconds, uint32_t milliseconds);

		/**
		* Destructor.
		*/
		virtual ~TimeSpan();

		/**
		* Ticks value.
		*/
		int64_t ticks;

	private:
		static int64_t TimeToTicks(uint32_t hours, uint32_t minutes, uint32_t seconds);
		static int64_t TimeToTicks(uint32_t days, uint32_t hours, uint32_t minutes, uint32_t seconds, uint32_t milliseconds);
	};
}