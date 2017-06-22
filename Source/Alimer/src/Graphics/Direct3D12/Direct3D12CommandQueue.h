/*
** Alimer - Copyright (C) Amer Koleci
**
** This file is subject to the terms and conditions defined in
** file 'LICENSE.md', which is part of this source code package.
*/

#pragma once

#include "Direct3D12CommandAllocatorPool.h"

namespace Alimer
{
	class Direct3D12CommandQueue final
	{
	public:
		Direct3D12CommandQueue(D3D12_COMMAND_LIST_TYPE commandListType);
		virtual ~Direct3D12CommandQueue();

		void Initialize(ID3D12Device* device);
		void Shutdown();

		inline bool IsReady() {
			return _commandQueue != nullptr;
		}

		uint64_t IncrementFence();
		bool IsFenceComplete(uint64_t fenceValue);
		void WaitForFence(uint64_t fenceValue);
		void WaitForIdle(void) { WaitForFence(IncrementFence()); }

		ID3D12CommandQueue* GetCommandQueue() { return _commandQueue; }
		uint64_t GetNextFenceValue() { return _nextFenceValue; }

		uint64_t ExecuteCommandList(ID3D12GraphicsCommandList* commandList);
		ID3D12CommandAllocator* RequestAllocator();
		void DiscardAllocator(uint64_t fenceValueForReset, ID3D12CommandAllocator* allocator);

	private:
		const D3D12_COMMAND_LIST_TYPE _commandListType;

		ID3D12CommandQueue* _commandQueue = nullptr;

		Direct3D12CommandAllocatorPool _allocatorPool;
		std::mutex _fenceMutex;
		std::mutex _eventMutex;

		// Lifetime of these objects is managed by the descriptor cache
		ID3D12Fence* _fence;
		uint64_t _nextFenceValue;
		uint64_t _lastCompletedFenceValue;
		HANDLE _fenceEventHandle;
	};
}
