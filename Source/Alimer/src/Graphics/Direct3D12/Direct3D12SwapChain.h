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

	class Direct3D12SwapChain final : public SwapChain
	{
	public:
		/**
		* Constructor.
		*/
		Direct3D12SwapChain(Direct3D12Device* device, Window* window, uint32_t frameCount, bool verticalSync);

		/**
		* Destructor.
		*/
		virtual ~Direct3D12SwapChain();

		void Present() override;

	private:
		Direct3D12Device* _device;
		uint32_t _presentInterval = 0;

		UINT _rtvDescriptorSize;

		ComPtr<IDXGISwapChain3> _swapChain;
		ComPtr<ID3D12DescriptorHeap> _renderTargetViewHeap;
	};
}
