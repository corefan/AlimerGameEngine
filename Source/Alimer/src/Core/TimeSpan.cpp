/*
** Alimer - Copyright (C) 2016-2017 Amer Koleci.
**
** This file is subject to the terms and conditions defined in
** file 'LICENSE.md', which is part of this source code package.
*/

#include "Core/TimeSpan.h"

namespace Alimer
{
	const double MillisecondsPerTick = 1.0 / TimeSpan::TicksPerMillisecond;
	const double SecondsPerTick = 1.0 / TimeSpan::TicksPerSecond; // 0.0001
	const double MinutesPerTick = 1.0 / TimeSpan::TicksPerMinute; // 1.6666666666667e-9
	const double HoursPerTick = 1.0 / TimeSpan::TicksPerHour; // 2.77777777777777778e-11
	const double DaysPerTick = 1.0 / TimeSpan::TicksPerDay; // 1.1574074074074074074e-12

	const int32_t MillisPerSecond = 1000;
	const int32_t MillisPerMinute = MillisPerSecond * 60; //     60,000
	const int32_t MillisPerHour = MillisPerMinute * 60;   //  3,600,000
	const int32_t MillisPerDay = MillisPerHour * 24;      // 86,400,000

	const int64_t MaxSeconds = std::numeric_limits<int64_t>::max() / TimeSpan::TicksPerSecond;
	const int64_t MinSeconds = std::numeric_limits<int64_t>::min() / TimeSpan::TicksPerSecond;
	const int64_t MaxMilliSeconds = std::numeric_limits<int64_t>::max() / TimeSpan::TicksPerMillisecond;
	const int64_t MinMilliSeconds = std::numeric_limits<int64_t>::min() / TimeSpan::TicksPerMillisecond;

	const TimeSpan TimeSpan::Zero = TimeSpan(0L);
	const TimeSpan TimeSpan::MaxValue = TimeSpan(std::numeric_limits<int64_t>::max());
	const TimeSpan TimeSpan::MinValue = TimeSpan(std::numeric_limits<int64_t>::min());

	TimeSpan::TimeSpan()
		: ticks(0)
	{

	}

	TimeSpan::TimeSpan(int64_t ticks_)
		: ticks(ticks_)
	{

	}

	TimeSpan::TimeSpan(uint32_t hours, uint32_t minutes, uint32_t seconds)
	{
		ticks = TimeToTicks(hours, minutes, seconds);
	}

	TimeSpan::TimeSpan(uint32_t days, uint32_t hours, uint32_t minutes, uint32_t seconds)
	{
		ticks = TimeToTicks(days, hours, minutes, seconds, 0);
	}

	TimeSpan::TimeSpan(uint32_t days, uint32_t hours, uint32_t minutes, uint32_t seconds, uint32_t milliseconds)
	{
		ticks = TimeToTicks(days, hours, minutes, seconds, milliseconds);
	}

	TimeSpan::~TimeSpan()
	{
	}

	int64_t TimeSpan::TimeToTicks(uint32_t hours, uint32_t minutes, uint32_t seconds)
	{
		// totalSeconds is bounded by 2^31 * 2^12 + 2^31 * 2^8 + 2^31,
		// which is less than 2^44, meaning we won't overflow totalSeconds.
		int64_t totalSeconds = (int64_t)(hours * 3600) + (int64_t)(minutes * 60) + (int64_t)seconds;
		if (totalSeconds > MaxSeconds || totalSeconds < MinSeconds)
		{
			ALIMER_LOGERROR("TimeToTicks: value to high");
		}

		return totalSeconds * TicksPerSecond;
	}

	int64_t TimeSpan::TimeToTicks(uint32_t days, uint32_t hours, uint32_t minutes, uint32_t seconds, uint32_t milliseconds)
	{
		int64_t totalMilliSeconds = ((int64_t)days * 3600 * 24 + (int64_t)hours * 3600 + (int64_t)minutes * 60 + seconds) * 1000 + milliseconds;
		if (totalMilliSeconds > MaxMilliSeconds || totalMilliSeconds < MinMilliSeconds)
		{
			ALIMER_LOGERROR("TimeToTicks: value to high");
		}

		return totalMilliSeconds * TimeSpan::TicksPerMillisecond;
	}
}