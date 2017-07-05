/*
** Alimer - Copyright (C) 2016-2017 Amer Koleci.
**
** This file is subject to the terms and conditions defined in
** file 'LICENSE.md', which is part of this source code package.
*/

#include "Direct3D12Shader.h"
#include "Direct3D12Device.h"
#include <spirv-cross/spirv_hlsl.hpp>

namespace Alimer
{
	Direct3D12Shader::Direct3D12Shader(Direct3D12Device* device, ShaderStage stage, uint32_t codeSize, const uint32_t* code)
		: Shader(device, stage, codeSize, code)
	{
		spirv_cross::CompilerHLSL compiler(code, codeSize);

		spirv_cross::CompilerHLSL::OptionsHLSL hlsl_options;
		hlsl_options.shader_model_major = 5;
		hlsl_options.shader_model_minor = 0;
		compiler.set_hlsl_options(hlsl_options);

		std::string hlslSource = compiler.compile();

		uint32_t compileFlags = 0;
#if defined(_DEBUG)
		// Enable better shader debugging with the graphics debugging tools.
		compileFlags |= D3DCOMPILE_DEBUG | D3DCOMPILE_SKIP_OPTIMIZATION;
#endif

		ComPtr<ID3DBlob> errors;

		const char* compileTarget = nullptr;
		const std::string entryPoint = "main";

		//D3D12_SHADER_BYTECODE* shader = nullptr;
		switch (stage)
		{
		case ShaderStage::Vertex:
			compileTarget = "vs_5_0";
			break;
		case ShaderStage::Fragment:
			compileTarget = "ps_5_0";
			break;
		}

		if (FAILED(D3DCompile(
			hlslSource.c_str(),
			hlslSource.length(),
			nullptr,
			nullptr,
			nullptr,
			entryPoint.c_str(),
			compileTarget,
			compileFlags,
			0,
			&_compiledShader,
			&errors
		))) {
			ALIMER_LOGERRORF("D3DCompile shader compile errors : %s", errors->GetBufferPointer());
			ALIMER_ASSERT(false);
		}

		_byteCode.pShaderBytecode = _compiledShader->GetBufferPointer();
		_byteCode.BytecodeLength = _compiledShader->GetBufferSize();
	}

	Direct3D12Shader::~Direct3D12Shader()
	{
	}
}