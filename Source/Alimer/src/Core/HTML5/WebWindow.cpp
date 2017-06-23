/*
** Alimer - Copyright (C) 2016-2017 Amer Koleci.
**
** This file is subject to the terms and conditions defined in
** file 'LICENSE.md', which is part of this source code package.
*/

#include "WebWindow.h"
#include "Engine/Application.h"

namespace Alimer
{
	namespace priv
	{
		WebWindow::WebWindow(uint32_t width, uint32_t height, const String& title, bool resizable, bool fullscreen)
		{
			int currentWidth, currentHeight, currentFullscreen;
			emscripten_get_canvas_size(&currentWidth, &currentHeight, &currentFullscreen);
			Platform::Print("Current %d, %d", currentWidth, currentHeight);
		}

		WebWindow::WebWindow(WindowHandle handle)
		{

		}

		WebWindow::~WebWindow()
		{
			
		}

		WindowHandle WebWindow::GetWindowHandle() const
		{
			return nullptr;
		}
	}
}