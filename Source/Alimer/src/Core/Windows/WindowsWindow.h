/*
** Alimer - Copyright (C) Amer Koleci
**
** This file is subject to the terms and conditions defined in
** file 'LICENSE.txt', which is part of this source code package.
*/

#pragma once

#include "Prerequisites.h"
#include "../WindowImpl.h"
#include <windows.h>

namespace Alimer
{
	namespace priv
	{
		class WindowsWindow : public WindowImpl
		{
		public:
			WindowsWindow(uint32_t width, uint32_t height, const String& title, bool resizable, bool fullscreen);
			 
			virtual ~WindowsWindow();

			WindowHandle GetWindowHandle() const override;

		private:
			DISALLOW_COPY_AND_ASSIGN(WindowsWindow);

			HWND _hwnd = nullptr;
			HDC _hdc = nullptr;
			HMONITOR _monitor = nullptr;
		};
	}
}