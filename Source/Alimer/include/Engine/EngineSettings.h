/*
** Alimer - Copyright (C) Amer Koleci
**
** This file is subject to the terms and conditions defined in
** file 'LICENSE.txt', which is part of this source code package.
*/

#pragma once

#include "Prerequisites.h"

namespace Alimer
{
	struct EngineSettings
	{
		String title = "Alimer";
		uint32_t width = 800;
		uint32_t height = 600;
		bool resizable = true;
		bool fullscreen = false;
		bool headless = false;
	};
}
