/*
** Alimer - Copyright (C) 2016-2017 Amer Koleci.
**
** This file is subject to the terms and conditions defined in
** file 'LICENSE.md', which is part of this source code package.
*/

#pragma once

#include "Prerequisites.h"
#include "Engine/Engine.h"
#include "Utils/CommandLine.h"

namespace Alimer
{
	/**
	* The application states.
	*/
	enum class ApplicationState : uint32_t
	{
		Uninitialzed,
		Running,
		Paused
	};

	class ALIMER_API Application : public RefCounted
	{
	public:
		/**
		* Constructor.
		*/
		Application();

		/**
		* Destructor.
		*/
		virtual ~Application();

		/**
		* Gets the single instance of the application.
		*
		* @return The single instance of the application.
		*/
		static Application* GetInstance();

		/**
		* Updates the app's clock and calls Update and Draw.
		*/
		void Tick();

		/**
		* Initialize the engine and run the main loop, then return the application exit code.
		*/
		int Run();

		/**
		* Gets the application state.
		*
		* @return The current application state.
		*/
		ApplicationState GetState() const {
			return _state;
		}

		/**
		* Gets the engine associated to this application.
		*/
		Engine* GetEngine() const {
			return _engine.Get();
		}

		CommandLine* GetArgs() const {
			return _commandLine.get();
		}

		void SetScreenSaverEnabled(bool value)
		{
			_screenSaverEnabled = value;
		}
		bool IsScreenSaverEnabled() const { return _screenSaverEnabled; }

	protected:
		/**
		* Setup before engine initialization and before running the main loop.
		*/
		virtual void Setup() { }

		/**
		* Setup after engine initialization and before running the main loop.
		*/
		virtual void Initialize() { }

	private:
		void InitializeBeforeRun();
		void Shutdown();

		int RunPlatformLoop();

	protected:
		ApplicationState _state;
		RefPtr<Engine> _engine;
		EngineSettings _settings;
		std::unique_ptr<CommandLine> _commandLine;
		bool _screenSaverEnabled = true;

	private:
		DISALLOW_COPY_AND_ASSIGN(Application);
	};
}
