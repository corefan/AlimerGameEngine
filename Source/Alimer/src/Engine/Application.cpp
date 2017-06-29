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
		, _commandLine(new CommandLine())
		, _engine(new Engine())
		, _exitCode(EXIT_SUCCESS)
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

	bool Application::InitializeBeforeRun()
	{
		// Initialize engine
		if (_state != ApplicationState::Uninitialzed) {
			Shutdown();
		}

		// If exit code was set during setup.
		if (!Setup() || _exitCode)
			return false;

		if (!_engine->Initialize(_settings))
		{
			Shutdown();
		}

		Initialize();

		_state = ApplicationState::Running;
		return true;
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
			_engine->RunFrame();
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
		_exitCode = RunPlatformLoop();
		return _exitCode;
	}
}