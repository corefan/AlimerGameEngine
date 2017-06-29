/*
** Alimer - Copyright (C) 2016-2017 Amer Koleci.
**
** This file is subject to the terms and conditions defined in
** file 'LICENSE.md', which is part of this source code package.
*/

#include "Direct3D12CommandBuffer.h"
#include "Direct3D12Device.h"
#include "Direct3D12Texture.h"

namespace Alimer
{
	Direct3D12RenderPassCommandEncoder::Direct3D12RenderPassCommandEncoder(Direct3D12CommandBuffer* commandBuffer, const RenderPassDescription& desc, ID3D12GraphicsCommandList* commandList)
		: RenderPassCommandEncoder(commandBuffer)
		, _commandList(commandList)
	{
		UINT numRenderTargetDescriptors = 0;
		for (uint32_t i = 0; i < MAX_COLOR_ATTACHMENTS; ++i)
		{
			const RenderPassColorAttachment& colorAttachment = desc.colorAttachments[i];
			if (colorAttachment.texture == nullptr)
				continue;

			auto d3dTexture = static_cast<Direct3D12Texture*>(colorAttachment.texture);
			commandBuffer->TransitionResource(d3dTexture, D3D12_RESOURCE_STATE_RENDER_TARGET, true);

			numRenderTargetDescriptors++;
			_renderTargetDescriptors[i] = d3dTexture->GetRenderTargetDescriptor();

			if (colorAttachment.loadAction == LoadAction::Clear)
			{
				const float clearColor[] = { 0.0f, 0.2f, 0.4f, 1.0f };
				_commandList->ClearRenderTargetView(_renderTargetDescriptors[i], clearColor, 0, nullptr);
			}
		}

		_commandList->OMSetRenderTargets(numRenderTargetDescriptors, _renderTargetDescriptors, FALSE, nullptr);
	}

	Direct3D12RenderPassCommandEncoder::~Direct3D12RenderPassCommandEncoder()
	{
		EndEncoding();
	}

	void Direct3D12RenderPassCommandEncoder::EndEncoding()
	{
		if (!_encoded)
		{
			_encoded = true;
		}
	}

	void Direct3D12RenderPassCommandEncoder::SetScissors(uint32_t scissorCount, const IntRect* pScissors)
	{
		static D3D12_RECT d3dScissors[D3D12_VIEWPORT_AND_SCISSORRECT_OBJECT_COUNT_PER_PIPELINE];

		for (uint32_t i = 0; i < scissorCount; ++i)
		{
			d3dScissors[i].left = static_cast<LONG>(pScissors[i].x);
			d3dScissors[i].top = static_cast<LONG>(pScissors[i].y);
			d3dScissors[i].right = static_cast<LONG>(pScissors[i].GetRight());
			d3dScissors[i].bottom = static_cast<LONG>(pScissors[i].GetBottom());
		}

		_commandList->RSSetScissorRects(scissorCount, d3dScissors);
	}

	void Direct3D12RenderPassCommandEncoder::SetScissor(IntRect scissor)
	{
		D3D12_RECT d3dScissor = {};

		d3dScissor.left = static_cast<LONG>(scissor.x);
		d3dScissor.top = static_cast<LONG>(scissor.y);
		d3dScissor.right = static_cast<LONG>(scissor.GetRight());
		d3dScissor.bottom = static_cast<LONG>(scissor.GetBottom());

		_commandList->RSSetScissorRects(1, &d3dScissor);
	}

	void Direct3D12RenderPassCommandEncoder::SetViewports(uint32_t viewportCount, const Viewport* pViewports)
	{
		static D3D12_VIEWPORT d3dViewports[D3D12_VIEWPORT_AND_SCISSORRECT_OBJECT_COUNT_PER_PIPELINE];

		for (uint32_t i = 0; i < viewportCount; ++i)
		{
			d3dViewports[i].TopLeftX = pViewports[i].x;
			d3dViewports[i].TopLeftY = pViewports[i].y;
			d3dViewports[i].Width = pViewports[i].width;
			d3dViewports[i].Height = pViewports[i].height;
			d3dViewports[i].MinDepth = pViewports[i].minDepth;
			d3dViewports[i].MaxDepth = pViewports[i].maxDepth;
		}

		_commandList->RSSetViewports(viewportCount, d3dViewports);
	}

	void Direct3D12RenderPassCommandEncoder::SetViewport(Viewport viewport)
	{
		D3D12_VIEWPORT d3dViewport = {};

		d3dViewport.TopLeftX = viewport.x;
		d3dViewport.TopLeftY = viewport.y;
		d3dViewport.Width = viewport.width;
		d3dViewport.Height = viewport.height;
		d3dViewport.MinDepth = viewport.minDepth;
		d3dViewport.MaxDepth = viewport.maxDepth;

		_commandList->RSSetViewports(1, &d3dViewport);
	}

	Direct3D12CommandBuffer::Direct3D12CommandBuffer(Direct3D12Device* device, D3D12_COMMAND_LIST_TYPE commandListType)
		: CommandBuffer()
		, _device(device)
		, _commandListType(commandListType)
		, _currentAllocator(nullptr)
	{
		_commandList = _device->AllocateCommandList(commandListType, &_currentAllocator);
	}

	Direct3D12CommandBuffer::~Direct3D12CommandBuffer()
	{
	}

	RenderPassCommandEncoderPtr Direct3D12CommandBuffer::OnCreateRenderCommandEncoder(const RenderPassDescription& desc)
	{
		return new Direct3D12RenderPassCommandEncoder(this, desc, _commandList);
	}

	/*void Direct3D12CommandBuffer::EndRenderPass()
	{
	TransitionResource(_boundFramebuffer, D3D12_RESOURCE_STATE_COMMON, true);
	}*/

	void Direct3D12CommandBuffer::Submit(bool waitForExecution)
	{
		// Restore default resource states.
		RestoreDefaultStates();

		auto& queue = _device->GetQueue(_commandListType);

		_fenceValue = queue.ExecuteCommandList(_commandList);
		queue.DiscardAllocator(_fenceValue, _currentAllocator);
		_currentAllocator = nullptr;

		if (waitForExecution)
		{
			_device->WaitForFence(_fenceValue);
		}

		// Free command list for late re execution.
		_device->FreeCommandList(_commandListType, _commandList);
	}

	void Direct3D12CommandBuffer::TransitionResource(const Direct3D12Resource* resource, D3D12_RESOURCE_STATES state, bool flushImmediate)
	{
		ALIMER_ASSERT(resource);
		ALIMER_ASSERT(resource->resource);

		auto stateEntry = _stateTracker.find(resource);
		if (stateEntry == _stateTracker.end())
		{
			_stateTracker.insert(std::make_pair(resource, resource->defaultState));
			stateEntry = _stateTracker.find(resource);
		}

		if (state != stateEntry->second)
		{
			D3D12_RESOURCE_BARRIER resourceBarrier;
			resourceBarrier.Transition.pResource = resource->resource.Get();
			resourceBarrier.Transition.StateBefore = stateEntry->second;
			resourceBarrier.Transition.StateAfter = state;
			resourceBarrier.Transition.Subresource = D3D12_RESOURCE_BARRIER_ALL_SUBRESOURCES;
			resourceBarrier.Type = D3D12_RESOURCE_BARRIER_TYPE_TRANSITION;
			resourceBarrier.Flags = D3D12_RESOURCE_BARRIER_FLAG_NONE;
			_pendingBarriers.insert(std::make_pair(resource, resourceBarrier));
			stateEntry->second = state;
		}

		if (flushImmediate)
		{
			FlushTransitions();
		}
	}

	void Direct3D12CommandBuffer::FlushTransitions()
	{
		if (!_pendingBarriers.size())
			return;

		// Copy pending barriers into flat vector.
		for (auto barrier : _pendingBarriers)
		{
			_barriers.push_back(barrier.second);
		}
		_pendingBarriers.clear();

		// Perform resource barriers.
		_commandList->ResourceBarrier((UINT)_barriers.size(), _barriers.data());
		_barriers.clear();
	}

	void Direct3D12CommandBuffer::RestoreDefaultStates()
	{
		for (auto state : _stateTracker)
		{
			TransitionResource(state.first, state.first->defaultState);
		}

		FlushTransitions();
		_stateTracker.clear();
	}
}
