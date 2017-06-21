/*
** Alimer - Copyright (C) 2016-2017 Amer Koleci.
**
** This file is subject to the terms and conditions defined in
** file 'LICENSE.md', which is part of this source code package.
*/

#pragma once

#include "Core/RefCounted.h"

namespace Alimer
{
	class Engine;

	/**
	* Frame Time class.
	*/
	class ALIMER_API Time : public RefCounted
	{
		friend class Engine;

	public:
		/**
		* Constructor.
		*/
		Time();

		/**
		* Destructor.
		*/
		virtual ~Time();

		/**
		* Reset elapsed time since initialize has been called.
		*/
		void ResetElapsedTime();

		/// Get elapsed time since the previous Update call.
		uint64_t GetElapsedTicks() const { return _elapsedTicks; }
		double GetElapsedSeconds() const { return TicksToSeconds(_elapsedTicks); }

		//* Get total time since the start of the program.
		uint64_t GetTotalTicks() const { return _totalTicks; }
		double GetTotalSeconds() const { return TicksToSeconds(_totalTicks); }

		/// Get total number of updates since start of the program.
		uint32 GetFrameCount() const { return _frameCount; }

		/// Get the current framerate.
		uint32_t GetFramesPerSecond() const { return _framesPerSecond; }

		/// Gets whether to use fixed or variable timestep mode.
		bool IsFixedTimeStep() const { return _isFixedTimeStep; }

		// Integer format represents time using 10,000,000 ticks per second.
		static const uint64_t TicksPerSecond = 10000000;

		/// Set whether to use fixed or variable timestep mode.
		void SetFixedTimeStep(bool isFixedTimestep) {  _isFixedTimeStep = isFixedTimestep; }

	private:
		static double TicksToSeconds(uint64_t ticks) { return static_cast<double>(ticks) / TicksPerSecond; }

		/**
		* Update timer state, calling the specified Update function the appropriate number of times.
		* Called by Engine.
		*/
		void Tick(Engine* engine);

	private:
		DISALLOW_COPY_AND_ASSIGN(Time);

		// Source timing data uses QPC units.
		uint64_t _qpcFrequency;
		uint64_t _qpcLastTime;
		uint64_t _qpcMaxDelta;

		// Derived timing data uses a canonical tick format.
		uint64_t _elapsedTicks = 0;
		uint64_t _totalTicks = 0;
		uint64_t _leftOverTicks = 0;

		// Members for tracking the framerate.
		uint32_t _frameCount = 0;
		uint32_t _framesPerSecond = 0;
		uint32_t _framesThisSecond = 0;
		uint64_t _qpcSecondCounter = 0;

		// Members for configuring fixed timestep mode.
		bool _isFixedTimeStep;
		uint64_t _targetElapsedTicks;
	};
}
