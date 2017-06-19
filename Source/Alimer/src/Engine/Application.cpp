/*
** Alimer - Copyright (C) 2016-2017 Amer Koleci.
**
** This file is subject to the terms and conditions defined in
** file 'LICENSE.md', which is part of this source code package.
*/

#include "Engine/Application.h"

namespace Alimer
{
	static Application* appInstance = nullptr;

	Application::Application()
		: _state(ApplicationState::Uninitialzed)
		, _engine(new Engine())
	{
		appInstance = this;
	}

	Application::~Application()
	{
		appInstance = nullptr;
	}

	Application* Application::GetInstance()
	{
		return appInstance;
	}

	void Application::InitializeBeforeRun()
	{
		// Initialize engine
		if (_state != ApplicationState::Uninitialzed) {
			Shutdown();
		}

		//Setup();
		if (!_engine->Initialize(_settings))
		{
			Shutdown();
		}

		Initialize();

		//ResetElapsedTime();
		_state = ApplicationState::Running;
	}

	void Application::Shutdown()
	{
		// Call user finalization.
		if (_state == ApplicationState::Uninitialzed)
			return;

		_state = ApplicationState::Uninitialzed;
	}

	void Application::Tick()
	{
		// update input
		if (_state == ApplicationState::Running)
		{
			//_engine->RunFrame();
		}
		else if (_state == ApplicationState::Paused)
		{
		}
	}

	int Application::Run()
	{
		if (_state != ApplicationState::Uninitialzed)
			return -1;

		// Run per platform game loop.
		int exitCode = RunPlatformLoop();

		return exitCode;
	}
}