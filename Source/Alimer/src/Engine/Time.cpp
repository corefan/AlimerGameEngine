/*
** Alimer - Copyright (C) 2016-2017 Amer Koleci.
**
** This file is subject to the terms and conditions defined in
** file 'LICENSE.md', which is part of this source code package.
*/

#include "Engine/Time.h"
#include "Core/Stopwatch.h"
#include "Engine/Engine.h"

namespace Alimer
{
	Time::Time()
		: _elapsedTicks(0)
		, _totalTicks(0)
		, _leftOverTicks(0)
		, _frameCount(0)
		, _framesPerSecond(0)
		, _framesThisSecond(0)
		, _qpcSecondCounter(0)
		, _isFixedTimeStep(false)
		, _targetElapsedTicks(TicksPerSecond / 60)
	{
		_qpcFrequency = Stopwatch::GetFrequency();
		_qpcLastTime = Stopwatch::GetTimestamp();

		// Initialize max delta to 1/10 of a second.
		_qpcMaxDelta = Stopwatch::GetFrequency() / 10;
	}

	Time::~Time()
	{
	}

	void Time::ResetElapsedTime()
	{
		_qpcLastTime = Stopwatch::GetTimestamp();
		_leftOverTicks = 0;
		_framesPerSecond = 0;
		_framesThisSecond = 0;
		_qpcSecondCounter = 0;
	}

	void Time::Tick(Engine* engine)
	{
		// Query the current time.
		uint64_t currentTime = Stopwatch::GetTimestamp();
		uint64_t timeDelta = currentTime - _qpcLastTime;

		_qpcLastTime = currentTime;
		_qpcSecondCounter += timeDelta;

		// Clamp excessively large time deltas (e.g. after paused in the debugger).
		if (timeDelta > _qpcMaxDelta)
		{
			timeDelta = _qpcMaxDelta;
		}

		// Convert QPC units into a canonical tick format. This cannot overflow due to the previous clamp.
		timeDelta *= TicksPerSecond;
		timeDelta /= _qpcFrequency;

		uint32_t lastFrameCount = _frameCount;

		if (_isFixedTimeStep)
		{
			// Fixed timestep update logic

			// If the app is running very close to the target elapsed time (within 1/4 of a millisecond) just clamp
			// the clock to exactly match the target value. This prevents tiny and irrelevant errors
			// from accumulating over time. Without this clamping, a game that requested a 60 fps
			// fixed update, running with vsync enabled on a 59.94 NTSC display, would eventually
			// accumulate enough tiny errors that it would drop a frame. It is better to just round 
			// small deviations down to zero to leave things running smoothly.

			if (std::abs(static_cast<int64_t>(timeDelta - _targetElapsedTicks)) < TicksPerSecond / 4000)
			{
				timeDelta = _targetElapsedTicks;
			}

			_leftOverTicks += timeDelta;

			while (_leftOverTicks >= _targetElapsedTicks)
			{
				_elapsedTicks = _targetElapsedTicks;
				_totalTicks += _targetElapsedTicks;
				_leftOverTicks -= _targetElapsedTicks;
				_frameCount++;

				engine->Update();
			}
		}
		else
		{
			// Variable timestep update logic.
			_elapsedTicks = timeDelta;
			_totalTicks += timeDelta;
			_leftOverTicks = 0;
			_frameCount++;

			engine->Update();
		}

		// Track the current framerate.
		if (_frameCount != lastFrameCount)
		{
			_framesThisSecond++;
		}

		if (_qpcSecondCounter >= _qpcFrequency)
		{
			_framesPerSecond = _framesThisSecond;
			_framesThisSecond = 0;
			_qpcSecondCounter %= _qpcFrequency;
		}
	}
}