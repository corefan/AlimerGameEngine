/*
** Alimer - Copyright (C) Amer Koleci
**
** This file is subject to the terms and conditions defined in
** file 'LICENSE.md', which is part of this source code package.
*/

#include "HelloWorld.h"
#include "Engine/Main.h"
#include "Core/TimeSpan.h"

HelloWorld::HelloWorld()
	: Application()
{
}

HelloWorld::~HelloWorld()
{
}

bool HelloWorld::Setup()
{
	_settings.graphicsDeviceType = GraphicsDeviceType::Direct3D12;
	//_settings.graphicsDeviceType = GraphicsDeviceType::Vulkan;

	return true;
}

ALIMER_MAIN(HelloWorld);