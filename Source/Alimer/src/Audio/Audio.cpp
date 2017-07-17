/*
** Alimer - Copyright (C) Amer Koleci
**
** This file is subject to the terms and conditions defined in
** file 'LICENSE.md', which is part of this source code package.
*/

#include "Audio/Audio.h"

namespace Alimer
{
	Audio::Audio(AudioDeviceType deviceType)
		: _deviceType(deviceType)
	{

	}

	Audio::~Audio()
	{
	}

	bool Audio::Initialize()
	{
		if (_initialized)
			return true;

		_initialized = true;
		return true;
	}

	void Audio::Pause()
	{

	}

	void Audio::Resume()
	{

	}

	std::set<AudioDeviceType> Audio::GetAvailableDrivers()
	{
		static std::set<AudioDeviceType> availableDrivers;

		if (availableDrivers.empty())
		{
			availableDrivers.insert(AudioDeviceType::Empty);

#if defined(ALIMER_SUPPORTS_XAUDIO2)
			availableDrivers.insert(AudioDeviceType::XAudio2);
#endif
		}

		return availableDrivers;
	}
}