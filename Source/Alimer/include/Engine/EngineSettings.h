/*
** Alimer - Copyright (C) 2016-2017 Amer Koleci.
**
** This file is subject to the terms and conditions defined in
** file 'LICENSE.md', which is part of this source code package.
*/

#pragma once

#include "Prerequisites.h"
#include "Graphics/GraphicsDevice.h"

namespace Alimer
{
	struct EngineSettings
	{
		GraphicsDeviceType graphicsDeviceType = GraphicsDeviceType::Default;

		String title = "Alimer";
		uint32_t width = 800;
		uint32_t height = 600;
		bool resizable = true;
		bool fullscreen = false;
		bool headless = false;
	};
}
