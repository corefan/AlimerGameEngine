/*
** Alimer - Copyright (C) Amer Koleci
**
** This file is subject to the terms and conditions defined in
** file 'LICENSE.md', which is part of this source code package.
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
			WindowsWindow(WindowHandle handle);

			virtual ~WindowsWindow();

			WindowHandle GetWindowHandle() const override;
			void GetSize(uint32_t* width, uint32_t* height) const override;

			void SetTitle(const String& title) override;

		private:
			static LRESULT CALLBACK GlobalWndProc(HWND hwnd, UINT msg, WPARAM wParam, LPARAM lParam);

		private:
			HWND _handle = nullptr;
			HMONITOR _monitor = nullptr;
			/// Stores the original event callback function of the control
			LONG_PTR _callback = 0;
		};
	}
}