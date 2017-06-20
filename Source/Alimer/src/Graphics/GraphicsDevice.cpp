/*
** Alimer - Copyright (C) 2016-2017 Amer Koleci.
**
** This file is subject to the terms and conditions defined in
** file 'LICENSE.md', which is part of this source code package.
*/

#include "Graphics/GraphicsDevice.h"

namespace Alimer
{
	GraphicsDevice::GraphicsDevice(GraphicsDeviceType deviceType)
		: _deviceType(deviceType)
	{
		
	}

	GraphicsDevice::~GraphicsDevice()
	{
	}

	bool GraphicsDevice::Initialize(Window* window)
	{
		if (_initialized)
			return true;

		_window = window;

		_initialized = true;
		return true;
	}

	bool GraphicsDevice::BeginFrame()
	{
		if (!_initialized)
			return false;

		return true;
	}

	void GraphicsDevice::EndFrame()
	{
		if (!_initialized)
			return;
	}

	std::set<GraphicsDeviceType> GraphicsDevice::GetAvailableDrivers()
	{
		static std::set<GraphicsDeviceType> availableDrivers;

		if (availableDrivers.empty())
		{
			availableDrivers.insert(GraphicsDeviceType::Empty);

#if defined(ALIMER_WINDOWS)
			availableDrivers.insert(GraphicsDeviceType::Direct3D12);
#endif

			// TODO: Add more
		}

		return availableDrivers;
	}
}