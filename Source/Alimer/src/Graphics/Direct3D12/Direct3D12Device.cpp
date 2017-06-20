/*
** Alimer - Copyright (C) 2016-2017 Amer Koleci.
**
** This file is subject to the terms and conditions defined in
** file 'LICENSE.md', which is part of this source code package.
*/

#include "Direct3D12Device.h"

namespace Alimer
{
	Direct3D12Device::Direct3D12Device()
		: GraphicsDevice(GraphicsDeviceType::Direct3D12)
	{
		UINT dxgiFactoryFlags = 0;

#if USE_D3D_DYNAMIC_LIB
		HMODULE dxgiDLL = ::LoadLibraryExW(L"dxgi.DLL", nullptr, 0x00000800 /* LOAD_LIBRARY_SEARCH_SYSTEM32 */);
		HMODULE d3d12DLL = ::LoadLibraryExW(L"d3d12.DLL", nullptr, 0x00000800 /* LOAD_LIBRARY_SEARCH_SYSTEM32 */);

		CreateDXGIFactory1 = (PFN_CREATE_DXGI_FACTORY)GetProcAddress(dxgiDLL, "CreateDXGIFactory1");

		if (CreateDXGIFactory1 == nullptr)
		{
			ALIMER_LOGERROR("Function CreateDXGIFactory1 not found in dxgi module.");
			return;
		}

		CreateDXGIFactory2 = (PFN_CREATE_DXGI_FACTORY2)GetProcAddress(dxgiDLL, "CreateDXGIFactory2");

		if (CreateDXGIFactory2 == nullptr)
		{
			ALIMER_LOGERROR("Function CreateDXGIFactory2 not found in dxgi module.");
			return;
		}

		// 
		D3D12CreateDevice = (PFN_D3D12_CREATE_DEVICE)GetProcAddress(d3d12DLL, "D3D12CreateDevice");

		if (D3D12CreateDevice == nullptr)
		{
			ALIMER_LOGERROR("Function D3D12CreateDevice not found.");
			return;
		}

		D3D12SerializeRootSignature = (PFN_D3D12_SERIALIZE_ROOT_SIGNATURE)GetProcAddress(d3d12DLL, "D3D12SerializeRootSignature");
		if (D3D12SerializeRootSignature == nullptr)
		{
			ALIMER_LOGERROR("Function D3D12SerializeRootSignature not found.");
			return;
		}

		D3D12GetDebugInterface = (PFN_D3D12_GET_DEBUG_INTERFACE)GetProcAddress(d3d12DLL, "D3D12GetDebugInterface");
		if (D3D12GetDebugInterface == nullptr)
		{
			ALIMER_LOGERROR("Function D3D12GetDebugInterface not found.");
			return;
		}
#endif

#ifdef _DEBUG
		// Enable the debug layer (requires the Graphics Tools "optional feature").
		// NOTE: Enabling the debug layer after device creation will invalidate the active device.
		{
			ComPtr<ID3D12Debug> debugController;
			if (SUCCEEDED(D3D12GetDebugInterface(IID_PPV_ARGS(&debugController))))
			{
				debugController->EnableDebugLayer();

				// Enable additional debug layers.
				dxgiFactoryFlags |= DXGI_CREATE_FACTORY_DEBUG;
			}
		}
#endif

		ThrowIfFailed(CreateDXGIFactory2(dxgiFactoryFlags, IID_PPV_ARGS(&_factory)));
	}

	Direct3D12Device::~Direct3D12Device()
	{
	}

	bool Direct3D12Device::Initialize(Window* window)
	{
		if (!Parent::Initialize(window))
			return false;

		ComPtr<IDXGIAdapter1> hardwareAdapter;
		GetHardwareAdapter(_factory.Get(), &hardwareAdapter);

		if (FAILED(D3D12CreateDevice(
			hardwareAdapter.Get(),
			D3D_FEATURE_LEVEL_11_0,
			IID_PPV_ARGS(&_d3d12Device))))
		{

			// Try with WARP
			ComPtr<IDXGIAdapter> warpAdapter;
			ThrowIfFailed(_factory->EnumWarpAdapter(IID_PPV_ARGS(&warpAdapter)));
			ThrowIfFailed(D3D12CreateDevice(warpAdapter.Get(), D3D_FEATURE_LEVEL_11_0, IID_PPV_ARGS(&_d3d12Device)));
		}

		// Initialize capabilities
		D3D12_FEATURE_DATA_FEATURE_LEVELS FeatureLevels = {};
		D3D_FEATURE_LEVEL FeatureLevelsList[] = {
			D3D_FEATURE_LEVEL_11_0,
			D3D_FEATURE_LEVEL_11_1,
			D3D_FEATURE_LEVEL_12_0,
			D3D_FEATURE_LEVEL_12_1,
		};
		FeatureLevels.NumFeatureLevels = ARRAYSIZE(FeatureLevelsList);
		FeatureLevels.pFeatureLevelsRequested = FeatureLevelsList;
		ThrowIfFailed(_d3d12Device->CheckFeatureSupport(D3D12_FEATURE_FEATURE_LEVELS, &FeatureLevels, sizeof(FeatureLevels)));

		// Describe and create the command queue.
		D3D12_COMMAND_QUEUE_DESC queueDesc = {};
		queueDesc.Flags = D3D12_COMMAND_QUEUE_FLAG_NONE;
		queueDesc.Type = D3D12_COMMAND_LIST_TYPE_DIRECT;

		ThrowIfFailed(_d3d12Device->CreateCommandQueue(&queueDesc, IID_PPV_ARGS(&_commandQueue)));

		// Describe and create the swap chain.
		DXGI_SWAP_CHAIN_DESC1 swapChainDesc = {};
		swapChainDesc.BufferCount = FrameCount;
		swapChainDesc.Width = window->GetWidth();
		swapChainDesc.Height = window->GetHeight();
		swapChainDesc.Format = DXGI_FORMAT_R8G8B8A8_UNORM;
		swapChainDesc.BufferUsage = DXGI_USAGE_RENDER_TARGET_OUTPUT;
		swapChainDesc.SwapEffect = DXGI_SWAP_EFFECT_FLIP_DISCARD;
		swapChainDesc.SampleDesc.Count = 1;

		// Swap chain needs the queue so that it can force a flush on it.
		ComPtr<IDXGISwapChain1> swapChain;
		ThrowIfFailed(_factory->CreateSwapChainForHwnd(
			_commandQueue.Get(),
			window->GetWindowHandle(),
			&swapChainDesc,
			nullptr,
			nullptr,
			&swapChain
		));

		// This sample does not support fullscreen transitions.
		ThrowIfFailed(_factory->MakeWindowAssociation(window->GetWindowHandle(), DXGI_MWA_NO_ALT_ENTER));

		ThrowIfFailed(swapChain.As(&_swapChain));

		return true;
	}

	bool Direct3D12Device::BeginFrame()
	{
		if (!Parent::BeginFrame())
			return false;

		return true;
	}

	void Direct3D12Device::EndFrame()
	{
		if (!_initialized)
			return;

		// Present the frame.
		const UINT presentInterval = 1;
		ThrowIfFailed(_swapChain->Present(presentInterval, 0));
	}

	// Helper function for acquiring the first available hardware adapter that supports Direct3D 12.
	// If no such adapter can be found, *ppAdapter will be set to nullptr.
	_Use_decl_annotations_ void Direct3D12Device::GetHardwareAdapter(IDXGIFactory2* pFactory, IDXGIAdapter1** ppAdapter)
	{
		ComPtr<IDXGIAdapter1> adapter;
		*ppAdapter = nullptr;

		for (UINT adapterIndex = 0; DXGI_ERROR_NOT_FOUND != pFactory->EnumAdapters1(adapterIndex, &adapter); ++adapterIndex)
		{
			DXGI_ADAPTER_DESC1 desc;
			adapter->GetDesc1(&desc);

			if (desc.Flags & DXGI_ADAPTER_FLAG_SOFTWARE)
			{
				// Don't select the Basic Render Driver adapter.
				// If you want a software adapter, pass in "/warp" on the command line.
				continue;
			}

			// Check to see if the adapter supports Direct3D 12, but don't create the
			// actual device yet.
			if (SUCCEEDED(D3D12CreateDevice(adapter.Get(), D3D_FEATURE_LEVEL_11_0, _uuidof(ID3D12Device), nullptr)))
			{
				break;
			}
		}

		*ppAdapter = adapter.Detach();
	}
}