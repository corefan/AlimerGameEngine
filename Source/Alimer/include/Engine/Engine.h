/*
** Alimer - Copyright (C) 2016-2017 Amer Koleci.
**
** This file is subject to the terms and conditions defined in
** file 'LICENSE.md', which is part of this source code package.
*/

#pragma once

#include "Prerequisites.h"
#include "Engine/EngineSettings.h"
#include "Core/Window.h"

namespace Alimer
{
	class ALIMER_API Engine
	{
	public:
		/**
		* Constructor.
		*/
		Engine();

		/**
		* Destructor.
		*/
		virtual ~Engine();

		/**
		* Gets the single instance of the Engine.
		*
		* @return The single instance of the Engine.
		*/
		static Engine* GetInstance();

		/**
		* Initializes Engine with given settings.
		*/
		bool Initialize(EngineSettings& newSettings);

		/// Return whether engine is running.
		bool IsRunning() const {
			return _running;
		}

	private:
		DISALLOW_COPY_AND_ASSIGN(Engine);

		bool _initialized = false;
		bool _running = false;
		bool _headless = false;

		/// Engine main Window
		std::unique_ptr<Window> _window;
	};
}
