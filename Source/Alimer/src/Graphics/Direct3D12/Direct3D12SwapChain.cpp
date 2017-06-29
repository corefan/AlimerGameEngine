/*
** Alimer - Copyright (C) 2016-2017 Amer Koleci.
**
** This file is subject to the terms and conditions defined in
** file 'LICENSE.md', which is part of this source code package.
*/

#include "Direct3D12SwapChain.h"
#include "Direct3D12Device.h"
#include "Direct3D12Texture.h"

namespace Alimer
{
	Direct3D12SwapChain::Direct3D12SwapChain(Direct3D12Device* device, Window* window, uint32_t frameCount, bool verticalSync)
		: SwapChain(device)
		, _presentInterval(verticalSync ? 1 : 0)
	{
		// Describe and create the swap chain.
		DXGI_SWAP_CHAIN_DESC1 swapChainDesc = {};
		swapChainDesc.BufferCount = frameCount;
		swapChainDesc.Width = window->GetWidth();
		swapChainDesc.Height = window->GetHeight();
		swapChainDesc.Format = DXGI_FORMAT_R8G8B8A8_UNORM;
		swapChainDesc.BufferUsage = DXGI_USAGE_RENDER_TARGET_OUTPUT;
		swapChainDesc.SwapEffect = DXGI_SWAP_EFFECT_FLIP_DISCARD;
		swapChainDesc.SampleDesc.Count = 1;

		// Swap chain needs the queue so that it can force a flush on it.
		ComPtr<IDXGISwapChain1> swapChain;
		ThrowIfFailed(device->GetDXGIFactory()->CreateSwapChainForHwnd(
			device->GetCommandQueue(),
			window->GetWindowHandle(),
			&swapChainDesc,
			nullptr,
			nullptr,
			&swapChain
		));

		// This sample does not support fullscreen transitions.
		ThrowIfFailed(device->GetDXGIFactory()->MakeWindowAssociation(window->GetWindowHandle(), DXGI_MWA_NO_ALT_ENTER));
		ThrowIfFailed(swapChain.As(&_swapChain));

		_backbufferTextures.resize(frameCount);
		_currentBackBufferIndex = _swapChain->GetCurrentBackBufferIndex();

		// Describe and create a render target view (RTV) descriptor heap.
		D3D12_DESCRIPTOR_HEAP_DESC rtvHeapDesc = {};
		rtvHeapDesc.NumDescriptors = frameCount;
		rtvHeapDesc.Type = D3D12_DESCRIPTOR_HEAP_TYPE_RTV;
		rtvHeapDesc.Flags = D3D12_DESCRIPTOR_HEAP_FLAG_NONE;
		ThrowIfFailed(device->GetD3D12Device()->CreateDescriptorHeap(&rtvHeapDesc, IID_PPV_ARGS(&_renderTargetViewHeap)));
		_rtvDescriptorSize = device->GetD3D12Device()->GetDescriptorHandleIncrementSize(D3D12_DESCRIPTOR_HEAP_TYPE_RTV);

		// Create a RTV for each frame.
		{
			D3D12_CPU_DESCRIPTOR_HANDLE renderTargetViewHandle = _renderTargetViewHeap->GetCPUDescriptorHandleForHeapStart();
			for (uint32_t n = 0; n < frameCount; ++n)
			{
				ComPtr<ID3D12Resource> renderTargetResource;
				ThrowIfFailed(swapChain->GetBuffer(n, IID_PPV_ARGS(&renderTargetResource)));
				device->GetD3D12Device()->CreateRenderTargetView(renderTargetResource.Get(), nullptr, renderTargetViewHandle);
				_backbufferTextures[n] = new Direct3D12Texture(device, renderTargetResource, renderTargetViewHandle);
				renderTargetViewHandle.ptr += _rtvDescriptorSize;
			}
		}
	}

	Direct3D12SwapChain::~Direct3D12SwapChain()
	{
	}

	void Direct3D12SwapChain::Present()
	{
		ThrowIfFailed(_swapChain->Present(_presentInterval, 0));

		_currentBackBufferIndex = _swapChain->GetCurrentBackBufferIndex();
	}
}