/*
** Alimer - Copyright (C) 2016-2017 Amer Koleci.
**
** This file is subject to the terms and conditions defined in
** file 'LICENSE.md', which is part of this source code package.
*/

#include "ShaderCompiler.h"

#include "glslang/Public/ShaderLang.h"
#include "SPIRV/GlslangToSpv.h"

namespace Alimer
{
	void InitResources(TBuiltInResource &resources)
	{
		resources.maxLights = 32;
		resources.maxClipPlanes = 6;
		resources.maxTextureUnits = 32;
		resources.maxTextureCoords = 32;
		resources.maxVertexAttribs = 64;
		resources.maxVertexUniformComponents = 4096;
		resources.maxVaryingFloats = 60;
		resources.maxVertexTextureImageUnits = 32;
		resources.maxCombinedTextureImageUnits = 80;
		resources.maxTextureImageUnits = 32;
		resources.maxFragmentUniformComponents = 4096;
		resources.maxDrawBuffers = 32;
		resources.maxVertexUniformVectors = 128;
		resources.maxVaryingVectors = 8;
		resources.maxFragmentUniformVectors = 16;
		resources.maxVertexOutputVectors = 16;
		resources.maxFragmentInputVectors = 15;
		resources.minProgramTexelOffset = -8;
		resources.maxProgramTexelOffset = 7;
		resources.maxClipDistances = 8;
		resources.maxComputeWorkGroupCountX = 65535;
		resources.maxComputeWorkGroupCountY = 65535;
		resources.maxComputeWorkGroupCountZ = 65535;
		resources.maxComputeWorkGroupSizeX = 1024;
		resources.maxComputeWorkGroupSizeY = 1024;
		resources.maxComputeWorkGroupSizeZ = 64;
		resources.maxComputeUniformComponents = 1024;
		resources.maxComputeTextureImageUnits = 16;
		resources.maxComputeImageUniforms = 8;
		resources.maxComputeAtomicCounters = 8;
		resources.maxComputeAtomicCounterBuffers = 1;
		resources.maxVaryingComponents = 60;
		resources.maxVertexOutputComponents = 64;
		resources.maxGeometryInputComponents = 64;
		resources.maxGeometryOutputComponents = 128;
		resources.maxFragmentInputComponents = 128;
		resources.maxImageUnits = 8;
		resources.maxCombinedImageUnitsAndFragmentOutputs = 8;
		resources.maxCombinedShaderOutputResources = 8;
		resources.maxImageSamples = 0;
		resources.maxVertexImageUniforms = 0;
		resources.maxTessControlImageUniforms = 0;
		resources.maxTessEvaluationImageUniforms = 0;
		resources.maxGeometryImageUniforms = 0;
		resources.maxFragmentImageUniforms = 8;
		resources.maxCombinedImageUniforms = 8;
		resources.maxGeometryTextureImageUnits = 16;
		resources.maxGeometryOutputVertices = 256;
		resources.maxGeometryTotalOutputComponents = 1024;
		resources.maxGeometryUniformComponents = 1024;
		resources.maxGeometryVaryingComponents = 64;
		resources.maxTessControlInputComponents = 128;
		resources.maxTessControlOutputComponents = 128;
		resources.maxTessControlTextureImageUnits = 16;
		resources.maxTessControlUniformComponents = 1024;
		resources.maxTessControlTotalOutputComponents = 4096;
		resources.maxTessEvaluationInputComponents = 128;
		resources.maxTessEvaluationOutputComponents = 128;
		resources.maxTessEvaluationTextureImageUnits = 16;
		resources.maxTessEvaluationUniformComponents = 1024;
		resources.maxTessPatchComponents = 120;
		resources.maxPatchVertices = 32;
		resources.maxTessGenLevel = 64;
		resources.maxViewports = 16;
		resources.maxVertexAtomicCounters = 0;
		resources.maxTessControlAtomicCounters = 0;
		resources.maxTessEvaluationAtomicCounters = 0;
		resources.maxGeometryAtomicCounters = 0;
		resources.maxFragmentAtomicCounters = 8;
		resources.maxCombinedAtomicCounters = 8;
		resources.maxAtomicCounterBindings = 1;
		resources.maxVertexAtomicCounterBuffers = 0;
		resources.maxTessControlAtomicCounterBuffers = 0;
		resources.maxTessEvaluationAtomicCounterBuffers = 0;
		resources.maxGeometryAtomicCounterBuffers = 0;
		resources.maxFragmentAtomicCounterBuffers = 1;
		resources.maxCombinedAtomicCounterBuffers = 1;
		resources.maxAtomicCounterBufferSize = 16384;
		resources.maxTransformFeedbackBuffers = 4;
		resources.maxTransformFeedbackInterleavedComponents = 64;
		resources.maxCullDistances = 8;
		resources.maxCombinedClipAndCullDistances = 8;
		resources.maxSamples = 4;

		resources.limits.nonInductiveForLoops = 1;
		resources.limits.whileLoops = 1;
		resources.limits.doWhileLoops = 1;
		resources.limits.generalUniformIndexing = 1;
		resources.limits.generalAttributeMatrixVectorIndexing = 1;
		resources.limits.generalVaryingIndexing = 1;
		resources.limits.generalSamplerIndexing = 1;
		resources.limits.generalVariableIndexing = 1;
		resources.limits.generalConstantMatrixVectorIndexing = 1;
	}

	TBuiltInResource Resources;

	struct glslangInitializer
	{
		glslangInitializer()
		{
			glslang::InitializeProcess();
			InitResources(Resources);
		}

		~glslangInitializer()
		{
			glslang::FinalizeProcess();
		}
	};

	glslangInitializer __glslangInitializer__;


	ShaderCompiler::ShaderCompiler()
	{

	}

	ShaderCompiler::~ShaderCompiler()
	{

	}

	bool ShaderCompiler::Compile(SourceShaderLanguage sourceLanguage, ShaderStage stage, const std::string& shaderSource, const std::string& entryPoint, const std::string& fileName, const std::string& preprocessorDefines)
	{
		EShLanguage shLanguage = EShLangVertex;
		switch (stage) {
		case ShaderStage::Vertex:
			shLanguage = EShLangVertex;
			break;
		case ShaderStage::TessellationControl:
			shLanguage = EShLangTessControl;
			break;
		case ShaderStage::TessellationEvaluation:
			shLanguage = EShLangTessEvaluation;
			break;
		case ShaderStage::Geometry:
			shLanguage = EShLangGeometry;
			break;
		case ShaderStage::Fragment:
			shLanguage = EShLangFragment;
			break;
		case ShaderStage::Compute:
			shLanguage = EShLangCompute;
			break;
		}

		_compileLog.clear();
		_compileLogVerbose.clear();
		_spirv.clear();

		glslang::TShader shader(shLanguage);
		glslang::TProgram program;

		// Populate sources.
		const char* sources[] = { shaderSource.c_str() };
		int sourceLengths[] = { (int)shaderSource.length() };
		const char* paths[] = { !fileName.empty() ? fileName.c_str() : "" };
		
		const char* poundExtension =
			"#extension GL_GOOGLE_include_directive : enable\n"
			"#extension GL_ARB_separate_shader_objects : enable\n"
			"#extension GL_ARB_shading_language_420pack : enable\n"
			;

		const std::string preamble = /*definesShaderCode +*/ poundExtension;

		shader.setPreamble(preamble.c_str());
		shader.setStringsWithLengthsAndNames(&sources[0], &sourceLengths[0], &paths[0], 1);
		shader.setEntryPoint(fileName.length() ? entryPoint.c_str() : "main");

		const EShMessages messages = static_cast<EShMessages>(EShMsgSpvRules | EShMsgVulkanRules | EShMsgCascadingErrors);
		bool parseResult;
		{
			parseResult = shader.parse(&Resources, 100, EEsProfile, false, true,
				messages);
		}

		_compileLog += shader.getInfoLog();
		_compileLogVerbose += shader.getInfoDebugLog();
		if (!parseResult) {
			return false;
		}

		program.addShader(&shader);
		program.mapIO();
		bool linkResult = program.link(messages);
		_compileLog += program.getInfoLog();
		_compileLogVerbose += program.getInfoDebugLog();
		if (!linkResult) {
			return false;
		}

		// Get the glslang intermediate form for our stage.
		auto intermediate = program.getIntermediate(shLanguage);
		if (!intermediate) {
			return false;
		}

		spv::SpvBuildLogger logger;
		glslang::GlslangToSpv(*intermediate, _spirv, &logger);
		_compileLog += logger.getAllMessages();

		return true;
	}

	std::vector<uint32_t> ShaderCompiler::AcquireSpirv()
	{
		return std::move(_spirv);
	}
}