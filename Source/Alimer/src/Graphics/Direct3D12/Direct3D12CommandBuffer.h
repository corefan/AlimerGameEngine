/*
** Alimer - Copyright (C) Amer Koleci
**
** This file is subject to the terms and conditions defined in
** file 'LICENSE.md', which is part of this source code package.
*/

#pragma once

#include "Direct3D12Prerequisites.h"
#include "Graphics/GraphicsDevice.h"

namespace Alimer
{
	class Direct3D12Device;
	class Direct3D12CommandBuffer;

	class Direct3D12RenderPassCommandEncoder final : public RenderPassCommandEncoder
	{
	public:
		Direct3D12RenderPassCommandEncoder(Direct3D12CommandBuffer* commandBuffer, const RenderPassDescription& desc, ID3D12GraphicsCommandList* commandList);
		~Direct3D12RenderPassCommandEncoder();

		void EndEncoding() override;

	private:
		ID3D12GraphicsCommandList* _commandList;
		D3D12_CPU_DESCRIPTOR_HANDLE _renderTargetDescriptors[MAX_COLOR_ATTACHMENTS];
		bool _encoded = false;
	};

	class Direct3D12CommandBuffer final : public CommandBuffer
	{
	public:
		/**
		* Constructor.
		*/
		Direct3D12CommandBuffer(Direct3D12Device* device, D3D12_COMMAND_LIST_TYPE commandListType);

		/**
		* Destructor.
		*/
		virtual ~Direct3D12CommandBuffer();

		RenderPassCommandEncoderPtr OnCreateRenderCommandEncoder(const RenderPassDescription& desc) override;

		void Submit(bool waitForExecution);

		/**
		* Do resource transition.
		* @param resource Resource to transition.
		* @param state States to transition.
		*/
		void TransitionResource(const Direct3D12Resource* resource, D3D12_RESOURCE_STATES state, bool flushImmediate = false);

		/**
		* Flush resource transitions.
		*/
		void FlushTransitions();

		/**
		* Restore default state of resources.
		*/
		void RestoreDefaultStates();

	private:
		Direct3D12Device* _device;

		D3D12_COMMAND_LIST_TYPE _commandListType;
		ID3D12GraphicsCommandList* _commandList;
		ID3D12CommandAllocator* _currentAllocator;

		std::map<const Direct3D12Resource*, D3D12_RESOURCE_STATES> _stateTracker;
		std::map<const Direct3D12Resource*, D3D12_RESOURCE_BARRIER> _pendingBarriers;
		std::vector<D3D12_RESOURCE_BARRIER> _barriers;

		uint64_t _fenceValue;
	};
}
