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
		_time = new Time();

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
			if (newSettings.externalWindowHandle)
			{
				_window.reset(new Window(newSettings.externalWindowHandle));
			}
			else
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
#if defined(ALIMER_WINDOWS)
						ALIMER_LOGINFO("Using DirectX12 graphics backend.");
						_graphicsDevice = new Direct3D12Device();
#else
						ALIMER_LOGINFO("DirectX12 backend not supported on given platform.");
#endif
					}

					// Initialize graphics device.
					if (!_graphicsDevice->Initialize())
					{
						return false;
					}

					// Create swap chain using our main window.
					_swapChain = _graphicsDevice->CreateSwapChain(_window.get(), 2, newSettings.verticalSync);
				}
			}
		}

		_time->ResetElapsedTime();
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

		_time->Tick(this);

		if (!_time->GetFrameCount())
			return;

		// Perform actual render.
		Render();
	}

	void Engine::Update()
	{

	}

	void Engine::Render()
	{
		// TODO: Perform rendering.
		auto commandBuffer = _graphicsDevice->CreateCommandBuffer();
		{
			RenderPassDescription passDesc = {};
			passDesc.colorAttachments[0].loadAction = LoadAction::Clear;
			passDesc.colorAttachments[0].storeAction = StoreAction::Store;
			passDesc.colorAttachments[0].texture = _swapChain->GetCurrentBackBuffer();

			auto encoder = commandBuffer->CreateRenderCommandEncoder(passDesc);
		}

		_graphicsDevice->Submit(commandBuffer);

		_swapChain->Present();
	}
}