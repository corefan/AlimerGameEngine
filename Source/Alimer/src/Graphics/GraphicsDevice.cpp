/*
** Alimer - Copyright (C) 2016-2017 Amer Koleci.
**
** This file is subject to the terms and conditions defined in
** file 'LICENSE.md', which is part of this source code package.
*/

#include "Graphics/GraphicsDevice.h"

#if defined(ALIMER_WINDOWS) || defined(ALIMER_WINMODERN)
#include "Graphics/Direct3D12/Direct3D12Device.h"
#endif

#if defined(ALIMER_WINDOWS) || defined(ALIMER_LINUX) || defined(ALIMER_ANDROID)
//#include "Graphics/Vulkan/VulkanDevice.h"
#endif

#ifdef _WIN32
// Prefer the high-performance GPU on switchable GPU systems
#include <windows.h>
extern "C" {
	__declspec(dllexport) DWORD NvOptimusEnablement = 1;
	__declspec(dllexport) int AmdPowerXpressRequestHighPerformance = 1;
}

#endif

namespace Alimer
{
	GraphicsDevice::GraphicsDevice(GraphicsDeviceType deviceType)
		: _deviceType(deviceType)
	{
		
	}

	GraphicsDevice::~GraphicsDevice()
	{
	}

	bool GraphicsDevice::Initialize(PhysicalDevice* physicalDevice)
	{
		if (_initialized)
			return true;

		_physicalDevice = physicalDevice;
		_initialized = true;

		return true;
	}

	std::set<GraphicsDeviceType> GraphicsDevice::GetAvailableDrivers()
	{
		static std::set<GraphicsDeviceType> availableDrivers;

		if (availableDrivers.empty())
		{
			availableDrivers.insert(GraphicsDeviceType::Empty);

#if defined(ALIMER_WINDOWS) || defined(ALIMER_LINUX) || defined(ALIMER_ANDROID)
			/*if (VulkanDevice::IsSupported())
			{
				availableDrivers.insert(GraphicsDeviceType::Vulkan);
			}*/
#endif

#if defined(ALIMER_WINDOWS) || defined(ALIMER_WINMODERN)
			if (Direct3D12Device::IsSupported())
			{
				availableDrivers.insert(GraphicsDeviceType::Direct3D12);
			}
#endif
		}

		return availableDrivers;
	}
}