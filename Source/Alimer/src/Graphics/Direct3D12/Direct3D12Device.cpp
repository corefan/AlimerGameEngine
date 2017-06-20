/*
** Alimer - Copyright (C) 2016-2017 Amer Koleci.
**
** This file is subject to the terms and conditions defined in
** file 'LICENSE.md', which is part of this source code package.
*/

#include "Direct3D12Device.h"

namespace Alimer
{
	Direct3D12Device::Direct3D12Device()
		: GraphicsDevice(GraphicsDeviceType::Direct3D12)
	{
		
	}

	Direct3D12Device::~Direct3D12Device()
	{
	}

	bool Direct3D12Device::Initialize(Window* window)
	{
		if (!Parent::Initialize(window))
			return false;

		return true;
	}

	bool Direct3D12Device::BeginFrame()
	{
		if (!Parent::BeginFrame())
			return false;

		return true;
	}

	void Direct3D12Device::EndFrame()
	{
		if (!_initialized)
			return;
	}
}