/*
** Alimer - Copyright (C) 2016-2017 Amer Koleci.
**
** This file is subject to the terms and conditions defined in
** file 'LICENSE.md', which is part of this source code package.
*/

#include "Engine/Application.h"
#include <emscripten.h>

static void emscriptenMainLoop(void)
{
	Alimer::Application* app = Alimer::Application::GetInstance();
	app->Tick();
}

namespace Alimer
{
	int Application::RunPlatformLoop()
	{
		if (_state != ApplicationState::Running)
		{
			InitializeBeforeRun();
		}

		int exitCode = EXIT_SUCCESS;

		emscripten_set_main_loop(emscriptenMainLoop, 0, 1);
		return exitCode;
	}
}