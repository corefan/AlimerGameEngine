/*
** Alimer - Copyright (C) 2016-2017 Amer Koleci.
**
** This file is subject to the terms and conditions defined in
** file 'LICENSE.md', which is part of this source code package.
*/

#include "Engine/Application.h"
#include <windows.h>

namespace Alimer
{
	int Application::RunPlatformLoop()
	{
		if (_state != ApplicationState::Running)
		{
			InitializeBeforeRun();
		}

		int exitCode = EXIT_SUCCESS;

		while (_engine->IsRunning())
		{
			MSG msg = {};

			// Process any messages in the queue.
			if (::PeekMessageW(&msg, nullptr, 0, 0, PM_REMOVE))
			{
				::TranslateMessage(&msg);
				::DispatchMessageW(&msg);

				exitCode = static_cast<int>(msg.wParam);
				// Check for quit.
				if (msg.message == WM_QUIT)
				{
					break;
				}
			}
			else
			{
				Tick();
			}

			// If we are done, then exit.
			if (_state == ApplicationState::Uninitialzed)
				break;
		}

		return exitCode;
	}
}