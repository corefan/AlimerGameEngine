/*
** Alimer - Copyright (C) 2016-2017 Amer Koleci.
**
** This file is subject to the terms and conditions defined in
** file 'LICENSE.md', which is part of this source code package.
*/

#include "Direct3D12Texture.h"
#include "Direct3D12Device.h"

namespace Alimer
{
	Direct3D12Texture::Direct3D12Texture(Direct3D12Device* device, ComPtr<ID3D12Resource> renderTargetResource, D3D12_CPU_DESCRIPTOR_HANDLE renderTargetViewHandle)
		: Texture()
		, _device(device)
		, _renderTargetViewHandle(renderTargetViewHandle)
	{
		resource = renderTargetResource;
	}

	Direct3D12Texture::~Direct3D12Texture()
	{
	}
}