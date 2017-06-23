/*
** Alimer - Copyright (C) Amer Koleci
**
** This file is subject to the terms and conditions defined in
** file 'LICENSE.md', which is part of this source code package.
*/

#pragma once

#include "Prerequisites.h"
#include "../WindowImpl.h"
#include <android/native_window.h>
#include <android/native_window_jni.h>

namespace Alimer
{
	namespace priv
	{
		class AndroidWindow : public WindowImpl
		{
		public:
			AndroidWindow(uint32_t width, uint32_t height, const String& title, bool resizable, bool fullscreen);
			AndroidWindow(WindowHandle handle);

			virtual ~AndroidWindow();

			WindowHandle GetWindowHandle() const override;

		private:
			ANativeWindow* _window = nullptr;
		};
	}
}