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
	/**
	* Core Engine class.
	*/
	class ALIMER_API Engine : public RefCounted
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

		/**
		* Run one frame.
		*/
		void RunFrame();

		/**
		* Return whether engine is running.
		*/
		bool IsRunning() const {
			return _running;
		}

		/**
		* Gets the main window instance.
		*/
		Window* GetWindow() const { 
			return _window.get(); 
		}

		/**
		* Gets the singleton GraphicsDevice instance.
		*/
		GraphicsDevice* GetGraphicsDevice() const { 
			return _graphicsDevice.Get();
		}

	private:
		DISALLOW_COPY_AND_ASSIGN(Engine);

		bool _initialized = false;
		bool _running = false;
		bool _headless = false;

		/// Engine main Window
		std::unique_ptr<Window> _window;

		/// Graphics device.
		RefPtr<GraphicsDevice> _graphicsDevice;
	};
}
