/*
** Alimer - Copyright (C) 2016-2017 Amer Koleci.
**
** This file is subject to the terms and conditions defined in
** file 'LICENSE.md', which is part of this source code package.
*/

#include "Direct3D12Shader.h"
#include "Direct3D12Device.h"

namespace Alimer
{
	Direct3D12Shader::Direct3D12Shader(Direct3D12Device* device, uint32_t codeSize, const uint32_t* code)
		: Shader(device, codeSize, code)
	{
		
	}

	Direct3D12Shader::~Direct3D12Shader()
	{
	}
}