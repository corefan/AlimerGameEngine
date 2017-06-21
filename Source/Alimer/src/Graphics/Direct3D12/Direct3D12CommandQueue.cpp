/*
** Alimer - Copyright (C) 2016-2017 Amer Koleci.
**
** This file is subject to the terms and conditions defined in
** file 'LICENSE.md', which is part of this source code package.
*/

#include "Direct3D12CommandQueue.h"

namespace Alimer
{
	Direct3D12CommandQueue::Direct3D12CommandQueue(D3D12_COMMAND_LIST_TYPE commandListType)
		: _commandListType(commandListType)
		, _nextFenceValue((uint64_t)commandListType << 56 | 1)
		, _lastCompletedFenceValue((uint64_t)commandListType << 56)
		, _allocatorPool(commandListType)
	{
	}

	Direct3D12CommandQueue::~Direct3D12CommandQueue()
	{
		Shutdown();
	}

	void Direct3D12CommandQueue::Initialize(ID3D12Device* device)
	{
		ALIMER_ASSERT(device != nullptr);
		ALIMER_ASSERT(!IsReady());
		ALIMER_ASSERT(_allocatorPool.Size() == 0);

		D3D12_COMMAND_QUEUE_DESC queueDesc = {};
		queueDesc.Type = _commandListType;
		queueDesc.NodeMask = 1;
		device->CreateCommandQueue(&queueDesc, IID_PPV_ARGS(&_commandQueue));
		//_commandQueue->SetName(L"CommandListManager::m_CommandQueue");

		ThrowIfFailed(device->CreateFence(0, D3D12_FENCE_FLAG_NONE, IID_PPV_ARGS(&_fence)));
		//_fence->SetName(L"CommandListManager::m_pFence");
		_fence->Signal((uint64_t)_commandListType << 56);

		_fenceEventHandle = CreateEvent(nullptr, false, false, nullptr);
		ALIMER_ASSERT(_fenceEventHandle != INVALID_HANDLE_VALUE);

		_allocatorPool.Initialize(device);

		ALIMER_ASSERT(IsReady());
	}

	void Direct3D12CommandQueue::Shutdown()
	{
		if (_commandQueue == nullptr)
			return;

		_allocatorPool.Shutdown();
		CloseHandle(_fenceEventHandle);

		_fence->Release();
		_fence = nullptr;

		_commandQueue->Release();
		_commandQueue = nullptr;
	}

	uint64_t Direct3D12CommandQueue::IncrementFence(void)
	{
		std::lock_guard<std::mutex> LockGuard(_fenceMutex);
		_commandQueue->Signal(_fence, _nextFenceValue);
		return _nextFenceValue++;
	}

	bool Direct3D12CommandQueue::IsFenceComplete(uint64_t fenceValue)
	{
		// Avoid querying the fence value by testing against the last one seen.
		// The max() is to protect against an unlikely race condition that could cause the last
		// completed fence value to regress.
		if (fenceValue > _lastCompletedFenceValue)
			_lastCompletedFenceValue = std::max(_lastCompletedFenceValue, _fence->GetCompletedValue());

		return fenceValue <= _lastCompletedFenceValue;
	}

	void Direct3D12CommandQueue::WaitForFence(uint64_t fenceValue)
	{
		if (IsFenceComplete(fenceValue))
			return;

		// TODO:  Think about how this might affect a multi-threaded situation.  Suppose thread A
		// wants to wait for fence 100, then thread B comes along and wants to wait for 99.  If
		// the fence can only have one event set on completion, then thread B has to wait for 
		// 100 before it knows 99 is ready.  Maybe insert sequential events?
		{
			std::lock_guard<std::mutex> LockGuard(_eventMutex);

			_fence->SetEventOnCompletion(fenceValue, _fenceEventHandle);
			WaitForSingleObject(_fenceEventHandle, INFINITE);
			_lastCompletedFenceValue = fenceValue;
		}
	}

	uint64_t Direct3D12CommandQueue::ExecuteCommandList(ID3D12GraphicsCommandList* commandList)
	{
		std::lock_guard<std::mutex> LockGuard(_fenceMutex);

		ThrowIfFailed(commandList->Close());

		// Kickoff the command list
		ID3D12CommandList* commandLists[] = { commandList };
		_commandQueue->ExecuteCommandLists(1, commandLists);

		// Signal the next fence value (with the GPU)
		_commandQueue->Signal(_fence, _nextFenceValue);

		// And increment the fence value.  
		return _nextFenceValue++;
	}

	ID3D12CommandAllocator* Direct3D12CommandQueue::RequestAllocator()
	{
		const uint64_t lastCompletedValue = _fence->GetCompletedValue();
		return _allocatorPool.RequestAllocator(lastCompletedValue);
	}

	void Direct3D12CommandQueue::DiscardAllocator(uint64_t fenceValueForReset, ID3D12CommandAllocator* allocator)
	{
		_allocatorPool.DiscardAllocator(fenceValueForReset, allocator);
	}
}