/*
** Alimer - Copyright (C) 2016-2017 Amer Koleci.
**
** This file is subject to the terms and conditions defined in
** file 'LICENSE.md', which is part of this source code package.
*/

#pragma once

#include "Prerequisites.h"
#include "Core/Window.h"
#include <set>

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
	class ALIMER_API GraphicsDevice
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
		bool Initialize(Window* window);

		/**
		* Begin frame rendering. Return true if device available and can render.
		*/
		bool BeginFrame();

		/**
		* End frame rendering and swap buffers.
		*/
		void EndFrame();

		/**
		* Gets the type of this device.
		*/
		GraphicsDeviceType GetDeviceType() const {
			return _deviceType;
		}

		bool IsInitialized() const {
			return _initialized;
		}

	private:
		DISALLOW_COPY_AND_ASSIGN(GraphicsDevice);

		GraphicsDeviceType _deviceType;
		bool _initialized = false;
		Window* _window = nullptr;
	};
}