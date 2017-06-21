/*
** Alimer - Copyright (C) Amer Koleci
**
** This file is subject to the terms and conditions defined in
** file 'LICENSE.txt', which is part of this source code package.
*/

#pragma once

#include "Direct3D12Prerequisites.h"

namespace Alimer
{
	class Direct3D12CommandAllocatorPool final
	{
	public:
		Direct3D12CommandAllocatorPool(D3D12_COMMAND_LIST_TYPE commandListType);
		virtual ~Direct3D12CommandAllocatorPool();

		void Initialize(ID3D12Device* device);
		void Shutdown();

		ID3D12CommandAllocator* RequestAllocator(uint64_t completedFenceValue);
		void DiscardAllocator(uint64_t fenceValue, ID3D12CommandAllocator* allocator);

		inline size_t Size() { return _allocatorPool.size(); }

	private:
		const D3D12_COMMAND_LIST_TYPE _commandListType;

		ID3D12Device* _d3d12Device = nullptr;
		std::vector<ID3D12CommandAllocator*> _allocatorPool;
		std::queue<std::pair<uint64_t, ID3D12CommandAllocator*>> _readyAllocators;
		std::mutex _allocatorMutex;
	};
}
