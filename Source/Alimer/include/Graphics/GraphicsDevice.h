/*
** Alimer - Copyright (C) 2016-2017 Amer Koleci.
**
** This file is subject to the terms and conditions defined in
** file 'LICENSE.md', which is part of this source code package.
*/

#pragma once

#include "Core/RefPtr.h"
#include "Graphics/PhysicalDevice.h"
#include "Graphics/SwapChain.h"
#include "Graphics/CommandBuffer.h"
#include "Graphics/Shader.h"

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
		friend class GraphicsResource;

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
		* Get all physical devices.
		*/
		std::vector<PhysicalDevice*> GetPhysicalDevices() const { return _physicalDevices; }

		/**
		* Initialize the device with given physical device.
		*/
		virtual bool Initialize(PhysicalDevice* physicalDevice);

		/**
		* Creates a SwapChain.
		*/
		virtual RefPtr<SwapChain> CreateSwapChain(Window* window, uint32_t frameCount = 2, bool verticalSync = true) = 0;

		/**
		* Creates a command buffer.
		*/
		virtual RefPtr<CommandBuffer> CreateCommandBuffer() = 0;

		/**
		* Submit given command buffer and eventually wait for its execution.
		*/
		virtual void Submit(RefPtr<CommandBuffer> commandBuffer, bool waitForExecution = false) = 0;

		/**
		* Creates a shader.
		*/
		virtual ShaderPtr CreateShader(uint32_t codeSize, const uint32_t* code) = 0;

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

	private:
		/// Add a GPU resource to keep track of.
		void AddGraphicsResource(GraphicsResource* resource);
		/// Remove a GPU resource. 
		void RemoveGraphicsResource(GraphicsResource* resource);

	protected:
		virtual void Finalize();

		GraphicsDeviceType _deviceType;
		bool _initialized = false;
		std::vector<PhysicalDevice*> _physicalDevices;
		PhysicalDevice* _physicalDevice = nullptr;

	private:
		std::vector<GraphicsResource*> _gpuResources;
		std::mutex _gpuResourceMutex;

		DISALLOW_COPY_AND_ASSIGN(GraphicsDevice);
	};
}