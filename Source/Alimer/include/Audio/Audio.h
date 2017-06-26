/*
** Alimer - Copyright (C) Amer Koleci
**
** This file is subject to the terms and conditions defined in
** file 'LICENSE.md', which is part of this source code package.
*/

#pragma once

#include "Core/RefCounted.h"

namespace Alimer
{
	enum class AudioDeviceType
	{
		Default,
		Empty,
		XAudio2,
		OpenAL
	};

	/**
	* Defines an abstract Audio system class.
	*/
	class ALIMER_API Audio : public RefCounted
	{
	protected:
		/**
		* Constructor.
		*/
		Audio(AudioDeviceType deviceType);

	public:
		/**
		* Destructor.
		*/
		virtual ~Audio();

		/**
		* Gets a set of all available backend drivers.
		*/
		static std::set<AudioDeviceType> GetAvailableDrivers();

		/**
		* Initialize the audio system.
		*/
		virtual bool Initialize();

		/**
		* Pause audio system.
		*/
		virtual void Pause();

		/**
		* Resume audio system.
		*/
		virtual void Resume();


		/**
		* Gets the backend device type.
		*/
		AudioDeviceType GetDeviceType() const {
			return _deviceType;
		}

		/**
		* Return whether the audio system has been initialized.
		*/
		bool IsInitialized() const {
			return _initialized;
		}

	protected:
		AudioDeviceType _deviceType;
		bool _initialized = false;

	private:
		DISALLOW_COPY_AND_ASSIGN(Audio);
	};
}
