/*
** Alimer - Copyright (C) Amer Koleci
**
** This file is subject to the terms and conditions defined in
** file 'LICENSE.txt', which is part of this source code package.
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
		typedef GraphicsDevice Parent;

		Direct3D12Device();

		virtual ~Direct3D12Device();

		bool Initialize(Window* window) override;
		bool BeginFrame() override;
		void EndFrame() override;

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

		UINT _rtvDescriptorSize;

		Direct3D12CommandQueue _graphicsQueue;
		Direct3D12CommandQueue _computeQueue;
		Direct3D12CommandQueue _copyQueue;

		std::mutex _commandListAllocationMutex;
		std::queue<ID3D12GraphicsCommandList*> _availableContexts[4];
		std::vector<ID3D12GraphicsCommandList*> _contextPool[4];

		// SwapChain
		ComPtr<IDXGISwapChain3> _swapChain;
		ComPtr<ID3D12DescriptorHeap> _renderTargetViewHeap;
		ComPtr<ID3D12Resource> _renderTargetResources[FrameCount];

		// Frame synchronization. Updated every frame
		UINT _renderTargetIndex;
		UINT _previousRenderTargetIndex;
		ComPtr<ID3D12GraphicsCommandList> _commandList;
	};
}