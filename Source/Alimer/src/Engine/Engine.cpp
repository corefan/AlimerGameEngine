/*
** Alimer - Copyright (C) 2016-2017 Amer Koleci.
**
** This file is subject to the terms and conditions defined in
** file 'LICENSE.md', which is part of this source code package.
*/

#include "Engine/Engine.h"

namespace Alimer
{
	static Engine* engineInstance = nullptr;

	Engine::Engine()
	{
		new Logger();
		engineInstance = this;
	}

	Engine::~Engine()
	{
		_initialized = false;
		_running = false;

		engineInstance = nullptr;
	}

	Engine* Engine::GetInstance()
	{
		ALIMER_ASSERT(engineInstance);
		return engineInstance;
	}

	bool Engine::Initialize(EngineSettings& newSettings)
	{
		if (_initialized)
			return true;

		_headless = newSettings.headless;
		ALIMER_LOGINFO("Initializing Alimer Engine %s...", ALIMER_VERSION_STR);

		// Initialize graphics & audio output
		if (!newSettings.headless)
		{
			/*if (newSettings.externalHandle)
			{
				_window.reset(new Window(newSettings.externalHandle, newSettings.externalConnection));
			}
			else*/
			{
				_window.reset(new Window(newSettings.width, newSettings.height, newSettings.title/*, WindowFlagBits::Shown | WindowFlagBits::Resizable*/));
			}
		}

		ALIMER_LOGINFO("Engine Initialize with success");
		_running = true;
		_initialized = true;
		return true;
	}
}