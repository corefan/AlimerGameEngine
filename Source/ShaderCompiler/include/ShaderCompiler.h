/*
** Alimer - Copyright (C) 2016-2017 Amer Koleci.
**
** This file is subject to the terms and conditions defined in
** file 'LICENSE.md', which is part of this source code package.
*/

#pragma once

#include "ShaderCompilerPrerequisites.h"
#include "Graphics/Shader.h"

namespace Alimer
{
	enum class SourceShaderLanguage : uint8_t {
		GLSL,
		HLSL
	};

	/**
	* Cross platform Shader compiler.
	*/
	class SHADER_COMPILER_API ShaderCompiler
	{
	public:
		ShaderCompiler();

		virtual ~ShaderCompiler();

		bool Compile(SourceShaderLanguage sourceLanguage, ShaderStage stage, const std::string& shaderSource, const std::string& entryPoint = "main", const std::string& fileName = "", const std::string& preprocessorDefines = "");

		std::vector<uint32_t> AcquireSpirv();

	private:
		DISALLOW_COPY_AND_ASSIGN(ShaderCompiler);

		std::string _compileLog;
		std::string _compileLogVerbose;
		std::vector<uint32_t> _spirv;
	};
}