/*
** Alimer - Copyright (C) 2016-2017 Amer Koleci.
**
** This file is subject to the terms and conditions defined in
** file 'LICENSE.md', which is part of this source code package.
*/

#include "Engine/Engine.h"

#if defined(ALIMER_WINDOWS)
#include "Graphics/Direct3D12/Direct3D12Device.h"
#endif

namespace Alimer
{
	static Engine* engineInstance = nullptr;

	Engine::Engine()
		: _logger(new Logger())
	{
		engineInstance = this;
	}

	Engine::~Engine()
	{
		// Close logger file.
		_logger->Close();

		_initialized = false;
		_running = false;

		engineInstance = nullptr;
	}

	Engine* Engine::GetInstance()
	{
		ALIMER_ASSERT(engineInstance);
		return engineInstance;
	}

	bool Engine::Initialize(EngineSettings& newSettings)
	{
		if (_initialized)
			return true;

		_headless = newSettings.headless;

		// Initialize logger
		_logger->Open("Alimer.log");

		ALIMER_LOGINFO("Initializing Alimer Engine %s...", ALIMER_VERSION_STR);

		// Initialize graphics & audio output
		if (!newSettings.headless)
		{
			/*if (newSettings.externalHandle)
			{
				_window.reset(new Window(newSettings.externalHandle, newSettings.externalConnection));
			}
			else*/
			{
				ALIMER_LOGINFO("Creating window with {%d, %d} size.", newSettings.width, newSettings.height);

				_window.reset(new Window(newSettings.width, newSettings.height, newSettings.title, newSettings.resizable, newSettings.fullscreen));

				if (newSettings.graphicsDeviceType == GraphicsDeviceType::Default)
				{
					auto availableDrivers = GraphicsDevice::GetAvailableDrivers();

					if (availableDrivers.find(GraphicsDeviceType::Direct3D12) != availableDrivers.end())
					{
						newSettings.graphicsDeviceType = GraphicsDeviceType::Direct3D12;
					}
					else if (availableDrivers.find(GraphicsDeviceType::Vulkan) != availableDrivers.end())
					{
						newSettings.graphicsDeviceType = GraphicsDeviceType::Vulkan;
					}

					else if (availableDrivers.find(GraphicsDeviceType::OpenGL) != availableDrivers.end())
					{
						newSettings.graphicsDeviceType = GraphicsDeviceType::OpenGL;
					}
					else
					{
						newSettings.graphicsDeviceType = GraphicsDeviceType::Empty;
					}

					switch (newSettings.graphicsDeviceType)
					{
					case GraphicsDeviceType::Empty:
						ALIMER_LOGINFO("Using empty graphics backend.");
						break;

					case GraphicsDeviceType::Direct3D12:
						ALIMER_LOGINFO("Using DirectX12 graphics backend.");
						_graphicsDevice = new Direct3D12Device();
						break;
					}

					// Initialize graphics device with our main window.
					if (!_graphicsDevice->Initialize(_window.get()))
					{
						return false;
					}
				}
			}
		}

		_running = true;
		_initialized = true;
		ALIMER_LOGINFO("Engine Initialize with success.");
		return true;
	}

	void Engine::RunFrame()
	{
		ALIMER_ASSERT(_initialized);

		// If not headless, and the graphics subsystem no longer has a window open, assume we should exit
		if (!_headless && !_graphicsDevice->IsInitialized())
			_running = false;

		if (!_running)
			return;

		if (!_graphicsDevice->BeginFrame())
			return;

		// TODO: Perform rendering.

		_graphicsDevice->EndFrame();
	}
}