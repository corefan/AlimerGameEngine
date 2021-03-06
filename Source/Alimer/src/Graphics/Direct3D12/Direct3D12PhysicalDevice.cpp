/*
** Alimer - Copyright (C) 2016-2017 Amer Koleci.
**
** This file is subject to the terms and conditions defined in
** file 'LICENSE.md', which is part of this source code package.
*/

#include "Direct3D12PhysicalDevice.h"

namespace Alimer
{
	Direct3D12PhysicalDevice::Direct3D12PhysicalDevice(ComPtr<IDXGIAdapter1> adapter)
		: _adapter(adapter)
	{
		
	}

	Direct3D12PhysicalDevice::~Direct3D12PhysicalDevice()
	{
	}
}