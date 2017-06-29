/*
** Alimer - Copyright (C) Amer Koleci
**
** This file is subject to the terms and conditions defined in
** file 'LICENSE.md', which is part of this source code package.
*/

#pragma once

#include "Direct3D12Prerequisites.h"
#include "Graphics/GraphicsDevice.h"
#include "Direct3D12CommandQueue.h"

namespace Alimer
{
	class Direct3D12Device final : public GraphicsDevice
	{
	public:
		/**
		* Constructor.
		*/
		Direct3D12Device();

		/**
		* Destructor.
		*/
		virtual ~Direct3D12Device();

		/**
		* Checks whether backend is supported.
		*/
		static bool IsSupported();

		bool Initialize() override;

		RefPtr<SwapChain> CreateSwapChain(Window* window, uint32_t frameCount, bool verticalSync) override;

		RefPtr<CommandBuffer> CreateCommandBuffer() override;
		void Submit(RefPtr<CommandBuffer> commandBuffer, bool waitForExecution) override;

		void WaitForFence(uint64_t fenceValue);

		ID3D12GraphicsCommandList* AllocateCommandList(D3D12_COMMAND_LIST_TYPE type, ID3D12CommandAllocator** allocator);
		void FreeCommandList(D3D12_COMMAND_LIST_TYPE type, ID3D12GraphicsCommandList* usedCommandList);

		IDXGIFactory4* GetDXGIFactory()  const {
			return _factory.Get();
		}

		ID3D12Device* GetD3D12Device() const {
			return _d3d12Device.Get();
		}

		Direct3D12CommandQueue& GetQueue(D3D12_COMMAND_LIST_TYPE type = D3D12_COMMAND_LIST_TYPE_DIRECT)
		{
			switch (type)
			{
			case D3D12_COMMAND_LIST_TYPE_COMPUTE: return _computeQueue;
			case D3D12_COMMAND_LIST_TYPE_COPY: return _copyQueue;
			default: return _graphicsQueue;
			}
		}

		ID3D12CommandQueue* GetCommandQueue()
		{
			return _graphicsQueue.GetCommandQueue();
		}

	private:
		void CreateNewCommandList(
			D3D12_COMMAND_LIST_TYPE type,
			ID3D12GraphicsCommandList** commandList,
			ID3D12CommandAllocator** allocator);

		static constexpr uint32_t FrameCount = 2;

		static void GetHardwareAdapter(_In_ IDXGIFactory2* pFactory, _Outptr_result_maybenull_ IDXGIAdapter1** ppAdapter);

		ComPtr<IDXGIFactory4> _factory;
		ComPtr<ID3D12Device> _d3d12Device;

		Direct3D12CommandQueue _graphicsQueue;
		Direct3D12CommandQueue _computeQueue;
		Direct3D12CommandQueue _copyQueue;

		std::mutex _commandListAllocationMutex;
		std::queue<ID3D12GraphicsCommandList*> _availableContexts[4];
		std::vector<ID3D12GraphicsCommandList*> _contextPool[4];
	};
}