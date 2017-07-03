/*
** Alimer - Copyright (C) 2016-2017 Amer Koleci.
**
** This file is subject to the terms and conditions defined in
** file 'LICENSE.md', which is part of this source code package.
*/

#include "Graphics/Shader.h"

namespace Alimer
{
	Shader::Shader(GraphicsDevice* device, ShaderStage stage, uint32_t codeSize, const uint32_t* code)
		: GraphicsResource(device, GraphicsResourceType::Shader)
		, _stage(stage)
		, spirv(code, code + codeSize)
	{
		
	}

	Shader::~Shader()
	{
	}
}