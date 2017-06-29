/*
** Alimer - Copyright (C) 2016-2017 Amer Koleci.
**
** This file is subject to the terms and conditions defined in
** file 'LICENSE.md', which is part of this source code package.
*/

#include "Engine/Application.h"
#include <objbase.h>
#include <windows.h>
#include <shellscalingapi.h>
#include <intrin.h>
#include <mmsystem.h>

#if defined(_MSC_VER)
extern "C" __declspec(dllimport) int __stdcall IsDebuggerPresent();
#endif

namespace Alimer
{
	HRESULT AlimerCoInitialize()
	{
#if	defined(ALIMER_WINMODERN)
		return S_OK;
#else
		HRESULT hr = CoInitializeEx(NULL, COINIT_APARTMENTTHREADED);
		if (hr == RPC_E_CHANGED_MODE) {
			hr = CoInitializeEx(NULL, COINIT_MULTITHREADED);
		}

		// S_FALSE means success, but someone else already initialized.
		// You still need to call CoUninitialize in this case!
		if (hr == S_FALSE) {
			return S_OK;
		}

		return hr;
#endif
	}

	int Application::RunPlatformLoop()
	{
#ifdef _DEBUG
		// Try to allocate console and don't print any errors.
		AllocConsole();
#endif

		if (_state != ApplicationState::Running)
		{
			if (!InitializeBeforeRun())
				return _exitCode;
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

#if	!defined(ALIMER_WINMODERN)
		CoUninitialize();
#endif

		return exitCode;
	}
}