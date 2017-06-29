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
		Finalize();
	}

	/// Sort resources by type.
	bool SortResources(GraphicsResource* x, GraphicsResource* y)
	{
		return x->GetResourceType() < y->GetResourceType();
	}

	void GraphicsDevice::Finalize()
	{
		if (!_initialized)
			return;

		{
			std::lock_guard<std::mutex> lock(_gpuResourceMutex);

			// Release all GPU objects that still exist
			std::sort(_gpuResources.begin(), _gpuResources.end(), SortResources);
			for (auto i = _gpuResources.begin(); i != _gpuResources.end(); ++i)
			{
				(*i)->Release();
			}

			_gpuResources.clear();
		}

		_initialized = false;
	}

	bool GraphicsDevice::Initialize(PhysicalDevice* physicalDevice)
	{
		if (_initialized)
			return true;

		_physicalDevice = physicalDevice;
		_initialized = true;

		return true;
	}

	void GraphicsDevice::AddGraphicsResource(GraphicsResource* resource)
	{
		std::lock_guard<std::mutex> lock(_gpuResourceMutex);
		_gpuResources.push_back(resource);
	}

	void GraphicsDevice::RemoveGraphicsResource(GraphicsResource* resource)
	{
		std::lock_guard<std::mutex> lock(_gpuResourceMutex);

		auto iter = std::find(_gpuResources.begin(), _gpuResources.end(), resource);
		if (iter != _gpuResources.end())
		{
			_gpuResources.erase(iter);
		}
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