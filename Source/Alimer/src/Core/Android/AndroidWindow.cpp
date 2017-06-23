/*
** Alimer - Copyright (C) 2016-2017 Amer Koleci.
**
** This file is subject to the terms and conditions defined in
** file 'LICENSE.md', which is part of this source code package.
*/

#include "AndroidWindow.h"
#include "Engine/Application.h"

namespace Alimer
{
	namespace priv
	{
		AndroidWindow::AndroidWindow(uint32_t width, uint32_t height, const String& title, bool resizable, bool fullscreen)
		{
		}

		AndroidWindow::AndroidWindow(WindowHandle handle)
			: _window(handle)
		{

		}

		AndroidWindow::~AndroidWindow()
		{
			
		}

		WindowHandle AndroidWindow::GetWindowHandle() const
		{
			return _window;
		}
	}
}