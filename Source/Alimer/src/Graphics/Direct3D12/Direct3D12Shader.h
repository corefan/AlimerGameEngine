/*
** Alimer - Copyright (C) Amer Koleci
**
** This file is subject to the terms and conditions defined in
** file 'LICENSE.md', which is part of this source code package.
*/

#pragma once

#include "Direct3D12Prerequisites.h"
#include "Graphics/Shader.h"

namespace Alimer
{
	class Direct3D12Device;

	class Direct3D12Shader final : public Shader
	{
	public:
		/**
		* Constructor.
		*/
		Direct3D12Shader(Direct3D12Device* device, uint32_t codeSize, const uint32_t* code);

		/**
		* Destructor.
		*/
		virtual ~Direct3D12Shader();

	private:
		std::string _hlslSource;
	};
}
