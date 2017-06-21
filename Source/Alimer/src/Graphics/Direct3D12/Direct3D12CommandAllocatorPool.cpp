/*
** Alimer - Copyright (C) 2016-2017 Amer Koleci.
**
** This file is subject to the terms and conditions defined in
** file 'LICENSE.md', which is part of this source code package.
*/

#include "Direct3D12CommandAllocatorPool.h"

namespace Alimer
{
	Direct3D12CommandAllocatorPool::Direct3D12CommandAllocatorPool(D3D12_COMMAND_LIST_TYPE commandListType)
		: _commandListType(commandListType)
	{
	}

	Direct3D12CommandAllocatorPool::~Direct3D12CommandAllocatorPool()
	{
		Shutdown();
	}

	void Direct3D12CommandAllocatorPool::Initialize(ID3D12Device* device)
	{
		_d3d12Device = device;
	}

	void Direct3D12CommandAllocatorPool::Shutdown()
	{
		for (size_t i = 0; i < _allocatorPool.size(); ++i)
		{
			_allocatorPool[i]->Release();
		}

		_allocatorPool.clear();
	}

	ID3D12CommandAllocator* Direct3D12CommandAllocatorPool::RequestAllocator(uint64_t CompletedFenceValue)
	{
		std::lock_guard<std::mutex> LockGuard(_allocatorMutex);

		ID3D12CommandAllocator* allocator = nullptr;

		if (!_readyAllocators.empty())
		{
			std::pair<uint64_t, ID3D12CommandAllocator*>& allocatorPair = _readyAllocators.front();

			if (allocatorPair.first <= CompletedFenceValue)
			{
				allocator = allocatorPair.second;
				ThrowIfFailed(allocator->Reset());
				_readyAllocators.pop();
			}
		}

		// If no allocator's were ready to be reused, create a new one
		if (allocator == nullptr)
		{
			ThrowIfFailed(_d3d12Device->CreateCommandAllocator(_commandListType, IID_PPV_ARGS(&allocator)));
			wchar_t AllocatorName[32];
			swprintf(AllocatorName, 32, L"CommandAllocator %zu", _allocatorPool.size());
			allocator->SetName(AllocatorName);
			_allocatorPool.push_back(allocator);
		}

		return allocator;
	}

	void Direct3D12CommandAllocatorPool::DiscardAllocator(uint64_t FenceValue, ID3D12CommandAllocator * Allocator)
	{
		std::lock_guard<std::mutex> LockGuard(_allocatorMutex);

		// That fence value indicates we are free to reset the allocator
		_readyAllocators.push(std::make_pair(FenceValue, Allocator));
	}
}