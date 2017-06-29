/*
** Alimer - Copyright (C) Amer Koleci
**
** This file is subject to the terms and conditions defined in
** file 'LICENSE.md', which is part of this source code package.
*/

#pragma once

#include "Direct3D12Prerequisites.h"
#include "Graphics/PhysicalDevice.h"

namespace Alimer
{
	class Direct3D12Device;

	class Direct3D12PhysicalDevice final : public PhysicalDevice
	{
	public:
		/**
		* Constructor.
		*/
		Direct3D12PhysicalDevice(ComPtr<IDXGIAdapter1> adapter);

		/**
		* Destructor.
		*/
		virtual ~Direct3D12PhysicalDevice();

		IDXGIAdapter1* GetAdapter() const {
			return _adapter.Get();
		}

	private:
		ComPtr<IDXGIAdapter1> _adapter;
	};
}
