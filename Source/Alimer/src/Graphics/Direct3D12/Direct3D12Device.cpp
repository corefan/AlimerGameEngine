/*
** Alimer - Copyright (C) 2016-2017 Amer Koleci.
**
** This file is subject to the terms and conditions defined in
** file 'LICENSE.md', which is part of this source code package.
*/

#include "Direct3D12Device.h"
#include "Direct3D12PhysicalDevice.h"
#include "Direct3D12CommandBuffer.h"
#include "Direct3D12SwapChain.h"

namespace Alimer
{
	Direct3D12Device::Direct3D12Device()
		: GraphicsDevice(GraphicsDeviceType::Direct3D12)
		, _graphicsQueue(D3D12_COMMAND_LIST_TYPE_DIRECT)
		, _computeQueue(D3D12_COMMAND_LIST_TYPE_COMPUTE)
		, _copyQueue(D3D12_COMMAND_LIST_TYPE_COPY)
	{
		UINT dxgiFactoryFlags = 0;

#ifdef _DEBUG
		if (D3D12GetDebugInterface)
		{
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
		}
#endif

		ThrowIfFailed(CreateDXGIFactory2(dxgiFactoryFlags, IID_PPV_ARGS(&_factory)));

		// Create adapters now
		ComPtr<IDXGIAdapter1> adapter;

		for (UINT adapterIndex = 0; DXGI_ERROR_NOT_FOUND != _factory->EnumAdapters1(adapterIndex, &adapter); ++adapterIndex)
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
				// Add to supported list
				_physicalDevices.emplace_back(new Direct3D12PhysicalDevice(adapter));
			}
		}
	}

	Direct3D12Device::~Direct3D12Device()
	{
		_graphicsQueue.Shutdown();
		_computeQueue.Shutdown();
		_copyQueue.Shutdown();
	}

	bool Direct3D12Device::IsSupported()
	{
		static bool availableCheck = false;
		static bool isAvailable = false;

		if (availableCheck)
			return isAvailable;

#if WINDOWS_USE_DYNAMIC_LIB
		HMODULE dxgiDLL = ::LoadLibraryExW(L"dxgi.DLL", nullptr, 0x00000800 /* LOAD_LIBRARY_SEARCH_SYSTEM32 */);
		HMODULE d3d12DLL = ::LoadLibraryExW(L"d3d12.DLL", nullptr, 0x00000800 /* LOAD_LIBRARY_SEARCH_SYSTEM32 */);

		CreateDXGIFactory1 = (PFN_CREATE_DXGI_FACTORY)GetProcAddress(dxgiDLL, "CreateDXGIFactory1");

		if (CreateDXGIFactory1 == nullptr)
		{
			availableCheck = true;
			isAvailable = false;
			return false;
		}

		CreateDXGIFactory2 = (PFN_CREATE_DXGI_FACTORY2)GetProcAddress(dxgiDLL, "CreateDXGIFactory2");

		if (CreateDXGIFactory2 == nullptr)
		{
			availableCheck = true;
			isAvailable = false;
			return false;
		}

		// 
		D3D12CreateDevice = (PFN_D3D12_CREATE_DEVICE)GetProcAddress(d3d12DLL, "D3D12CreateDevice");

		if (D3D12CreateDevice == nullptr)
		{
			availableCheck = true;
			isAvailable = false;
			return false;
		}

		D3D12SerializeRootSignature = (PFN_D3D12_SERIALIZE_ROOT_SIGNATURE)GetProcAddress(d3d12DLL, "D3D12SerializeRootSignature");
		if (D3D12SerializeRootSignature == nullptr)
		{
			availableCheck = true;
			isAvailable = false;
			return false;
		}

		// Optional entries
		D3D12GetDebugInterface = (PFN_D3D12_GET_DEBUG_INTERFACE)GetProcAddress(d3d12DLL, "D3D12GetDebugInterface");
#endif

		availableCheck = isAvailable = true;
		return isAvailable;
	}

	bool Direct3D12Device::Initialize(PhysicalDevice* physicalDevice)
	{
		if (!GraphicsDevice::Initialize(physicalDevice))
			return false;

		IDXGIAdapter1* adapter = static_cast<Direct3D12PhysicalDevice*>(physicalDevice)->GetAdapter();

		if (FAILED(D3D12CreateDevice(
			adapter,
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

		// Init command queue's
		_graphicsQueue.Initialize(_d3d12Device.Get());
		_computeQueue.Initialize(_d3d12Device.Get());
		_copyQueue.Initialize(_d3d12Device.Get());

		return true;
	}

	RefPtr<SwapChain> Direct3D12Device::CreateSwapChain(Window* window, uint32_t frameCount, bool verticalSync)
	{
		return new Direct3D12SwapChain(this, window, frameCount, verticalSync);
	}

	RefPtr<CommandBuffer> Direct3D12Device::CreateCommandBuffer()
	{
		return new Direct3D12CommandBuffer(this, D3D12_COMMAND_LIST_TYPE_DIRECT);
	}

	void Direct3D12Device::Submit(RefPtr<CommandBuffer> commandBuffer, bool waitForExecution)
	{
		auto d3dCommandBuffer = static_cast<Direct3D12CommandBuffer*>(commandBuffer.Get());
		d3dCommandBuffer->Submit(waitForExecution);
	}

	void Direct3D12Device::WaitForFence(uint64_t fenceValue)
	{
		Direct3D12CommandQueue& producer = GetQueue((D3D12_COMMAND_LIST_TYPE)(fenceValue >> 56));
		producer.WaitForFence(fenceValue);
	}

	ID3D12GraphicsCommandList* Direct3D12Device::AllocateCommandList(D3D12_COMMAND_LIST_TYPE type, ID3D12CommandAllocator** allocator)
	{
		std::lock_guard<std::mutex> LockGuard(_commandListAllocationMutex);

		std::queue<ID3D12GraphicsCommandList*>& availableContexts = _availableContexts[type];

		ID3D12GraphicsCommandList* commandList = nullptr;
		if (availableContexts.empty())
		{
			CreateNewCommandList(type, &commandList, allocator);
			_contextPool[type].emplace_back(commandList);
		}
		else
		{
			// We only call Reset() on previously freed contexts.  The command list persists, but we must
			// request a new allocator.
			commandList = availableContexts.front();
			availableContexts.pop();

			*allocator = GetQueue(type).RequestAllocator();
			commandList->Reset(*allocator, nullptr);
		}

		return commandList;
	}

	void Direct3D12Device::FreeCommandList(D3D12_COMMAND_LIST_TYPE type, ID3D12GraphicsCommandList* usedCommandList)
	{
		ALIMER_ASSERT(usedCommandList != nullptr);
		std::lock_guard<std::mutex> LockGuard(_commandListAllocationMutex);
		_availableContexts[type].push(usedCommandList);
	}

	void Direct3D12Device::CreateNewCommandList(
		D3D12_COMMAND_LIST_TYPE type,
		ID3D12GraphicsCommandList** commandList,
		ID3D12CommandAllocator** allocator)
	{
		ALIMER_ASSERT_MSG(type != D3D12_COMMAND_LIST_TYPE_BUNDLE, "Bundles are not yet supported");
		switch (type)
		{
		case D3D12_COMMAND_LIST_TYPE_DIRECT: *allocator = _graphicsQueue.RequestAllocator(); break;
		case D3D12_COMMAND_LIST_TYPE_BUNDLE: break;
		case D3D12_COMMAND_LIST_TYPE_COMPUTE: *allocator = _computeQueue.RequestAllocator(); break;
		case D3D12_COMMAND_LIST_TYPE_COPY: *allocator = _copyQueue.RequestAllocator(); break;
		}

		ThrowIfFailed(_d3d12Device->CreateCommandList(1, type, *allocator, nullptr, IID_PPV_ARGS(commandList)));
	}
}