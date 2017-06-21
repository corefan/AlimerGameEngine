/*
** Alimer - Copyright (C) 2016-2017 Amer Koleci.
**
** This file is subject to the terms and conditions defined in
** file 'LICENSE.md', which is part of this source code package.
*/

#pragma once

#include <set>
#include "Core/RefCounted.h"
#include "Core/Window.h"
#include "Graphics/CommandBuffer.h"

namespace Alimer
{
	enum class GraphicsDeviceType
	{
		Default,
		Empty,
		Vulkan,
		Direct3D12,
		OpenGL,
	};

	/**
	* Graphics Device.
	*/
	class ALIMER_API GraphicsDevice : public RefCounted
	{
	protected:
		/**
		* Constructor.
		*/
		GraphicsDevice(GraphicsDeviceType deviceType);

	public:
		/**
		* Destructor.
		*/
		virtual ~GraphicsDevice();

		/**
		* Gets a set of all available backend drivers.
		*/
		static std::set<GraphicsDeviceType> GetAvailableDrivers();

		/**
		* Initialize device with given window.
		* @param window The window where to perform rendering.
		*/
		virtual bool Initialize(Window* window);

		/**
		* Begin frame rendering. Return true if device available and can render.
		*/
		virtual bool BeginFrame();

		/**
		* End frame rendering and swap buffers.
		*/
		virtual void EndFrame();

		/**
		* Creates a command buffer.
		*/
		virtual RefPtr<CommandBuffer> CreateCommandBuffer() = 0;

		virtual void Submit(RefPtr<CommandBuffer> commandBuffer, bool waitForExecution = false) = 0;

		/**
		* Gets the type of this device.
		*/
		GraphicsDeviceType GetDeviceType() const {
			return _deviceType;
		}

		/**
		* Return whether the device has been initialized.
		*/
		bool IsInitialized() const {
			return _initialized;
		}

	protected:
		DISALLOW_COPY_AND_ASSIGN(GraphicsDevice);

		GraphicsDeviceType _deviceType;
		bool _initialized = false;
		Window* _window = nullptr;
	};
}