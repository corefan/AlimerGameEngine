/*
** Alimer - Copyright (C) Amer Koleci
**
** This file is subject to the terms and conditions defined in
** file 'LICENSE.md', which is part of this source code package.
*/

#pragma once

#include "Prerequisites.h"
#include "Core/Windows/WindowsHeaders.h"

#include <d3d12.h>
#include <dxgi1_4.h>

namespace Alimer
{
#if WINDOWS_USE_DYNAMIC_LIB
	typedef HRESULT(WINAPI* PFN_CREATE_DXGI_FACTORY)(REFIID _riid, void** _factory);
	typedef HRESULT(WINAPI* PFN_CREATE_DXGI_FACTORY2)(UINT Flags, REFIID riid, _COM_Outptr_ void **ppFactory);

	static PFN_D3D12_CREATE_DEVICE            D3D12CreateDevice;
	static PFN_CREATE_DXGI_FACTORY            CreateDXGIFactory1;
	static PFN_CREATE_DXGI_FACTORY2			  CreateDXGIFactory2;
	static PFN_D3D12_GET_DEBUG_INTERFACE      D3D12GetDebugInterface;
	static PFN_D3D12_SERIALIZE_ROOT_SIGNATURE D3D12SerializeRootSignature;
#endif

	struct Direct3D12Resource
	{
		ComPtr<ID3D12Resource> resource;
		D3D12_RESOURCE_STATES supportedStates = D3D12_RESOURCE_STATE_COMMON;
		D3D12_RESOURCE_STATES defaultState = D3D12_RESOURCE_STATE_COMMON;
	};
}