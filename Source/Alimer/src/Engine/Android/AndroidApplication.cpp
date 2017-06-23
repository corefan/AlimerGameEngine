/*
** Alimer - Copyright (C) 2016-2017 Amer Koleci.
**
** This file is subject to the terms and conditions defined in
** file 'LICENSE.md', which is part of this source code package.
*/

#include "Engine/Application.h"
#include <jni.h>
#include <android/asset_manager.h>
#include <android/asset_manager_jni.h>


namespace Alimer
{
	int Application::RunPlatformLoop()
	{
		int exitCode = EXIT_SUCCESS;
		return exitCode;
	}
}