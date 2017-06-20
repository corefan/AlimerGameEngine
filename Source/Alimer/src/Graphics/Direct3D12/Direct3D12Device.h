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
	class Direct3D12Device final : public GraphicsDevice
	{
	public:
		typedef GraphicsDevice Parent;

		Direct3D12Device();

		virtual ~Direct3D12Device();

		bool Initialize(Window* window) override;
		bool BeginFrame() override;
		void EndFrame() override;
	};
}