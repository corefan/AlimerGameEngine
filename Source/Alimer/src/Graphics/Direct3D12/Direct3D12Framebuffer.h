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
	class Direct3D12Device;

	class Direct3D12Framebuffer final : public Framebuffer, public Direct3D12Resource
	{
	public:
		/**
		* Constructor.
		*/
		Direct3D12Framebuffer(Direct3D12Device* device, ComPtr<ID3D12Resource> renderTargetResource, D3D12_CPU_DESCRIPTOR_HANDLE renderTargetViewHandle);

		/**
		* Destructor.
		*/
		virtual ~Direct3D12Framebuffer();

		D3D12_CPU_DESCRIPTOR_HANDLE GetRenderTargetDescriptor() const {
			return _renderTargetViewHandle;
		}

	private:
		Direct3D12Device* _device;
		D3D12_CPU_DESCRIPTOR_HANDLE _renderTargetViewHandle;
	};
}
