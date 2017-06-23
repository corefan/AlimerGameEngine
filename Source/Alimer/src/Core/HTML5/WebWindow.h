/*
** Alimer - Copyright (C) Amer Koleci
**
** This file is subject to the terms and conditions defined in
** file 'LICENSE.md', which is part of this source code package.
*/

#pragma once

#include "Prerequisites.h"
#include "../WindowImpl.h"
#include <emscripten.h>

namespace Alimer
{
	namespace priv
	{
		class WebWindow : public WindowImpl
		{
		public:
			WebWindow(uint32_t width, uint32_t height, const String& title, bool resizable, bool fullscreen);
			WebWindow(WindowHandle handle);

			virtual ~WebWindow();

			WindowHandle GetWindowHandle() const override;

		private:
		};
	}
}