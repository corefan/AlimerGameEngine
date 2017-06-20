/*
** Alimer - Copyright (C) Amer Koleci
**
** This file is subject to the terms and conditions defined in
** file 'LICENSE.txt', which is part of this source code package.
*/

#pragma once

#include "Direct3D12Prerequisites.h"
#include "Graphics/GraphicsDevice.h"

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

	private:
		void NextFenceValue();
		void WaitForFenceValue(UINT64 value);

		static constexpr uint32_t FrameCount = 2;

		static void GetHardwareAdapter(_In_ IDXGIFactory2* pFactory, _Outptr_result_maybenull_ IDXGIAdapter1** ppAdapter);

		ComPtr<IDXGIFactory4> _factory;
		ComPtr<ID3D12Device> _d3d12Device;
		ComPtr<ID3D12CommandQueue> _commandQueue;

		UINT _rtvDescriptorSize;

		// Sync primitives/fence
		ComPtr<ID3D12Fence> _fence;
		HANDLE _fenceEvent;
		UINT64 _fenceValue = 0;
		UINT64 _lastFenceRenderTargetWasUsed[FrameCount];

		// SwapChain
		ComPtr<IDXGISwapChain3> _swapChain;
		ComPtr<ID3D12DescriptorHeap> _renderTargetViewHeap;
		ComPtr<ID3D12Resource> _renderTargetResources[FrameCount];

		// Frame synchronization. Updated every frame
		UINT _renderTargetIndex;
		UINT _previousRenderTargetIndex;
		ComPtr<ID3D12GraphicsCommandList> _commandList;
		ComPtr<ID3D12CommandAllocator> _commandAllocator;
	};
}