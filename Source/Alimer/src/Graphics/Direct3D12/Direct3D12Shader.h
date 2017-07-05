/*
** Alimer - Copyright (C) Amer Koleci
**
** This file is subject to the terms and conditions defined in
** file 'LICENSE', which is part of this source code package.
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
		Direct3D12Shader(Direct3D12Device* device, ShaderStage stage, uint32_t codeSize, const uint32_t* code);

		/**
		* Destructor.
		*/
		virtual ~Direct3D12Shader();

		D3D12_SHADER_BYTECODE GetByteCode() const {
			return _byteCode;
		}

	private:
		ComPtr<ID3DBlob> _compiledShader;
		D3D12_SHADER_BYTECODE _byteCode;
	};
}
