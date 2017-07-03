/*
** Alimer - Copyright (C) Amer Koleci
**
** This file is subject to the terms and conditions defined in
** file 'LICENSE.md', which is part of this source code package.
*/

#include "HelloWorld.h"
#include "Engine/Main.h"
#include "ShaderCompiler.h"

HelloWorld::HelloWorld()
	: Application()
{
}

HelloWorld::~HelloWorld()
{
}

bool HelloWorld::Setup()
{
	_settings.graphicsDeviceType = GraphicsDeviceType::Direct3D12;
	//_settings.graphicsDeviceType = GraphicsDeviceType::Vulkan;

	return true;
}

void HelloWorld::Initialize()
{
	ShaderCompiler compiler;
	if (!compiler.Compile(SourceShaderLanguage::GLSL, ShaderStage::Vertex, R"(
        #version 450
        layout(location = 0) in vec4 pos;
        void main() {
            gl_Position = pos;
        })"))
	{

	}

	auto spirv_vertex = compiler.AcquireSpirv();
	_engine->GetGraphicsDevice()->CreateShader(ShaderStage::Vertex, spirv_vertex.size(), spirv_vertex.data());
}

ALIMER_MAIN(HelloWorld);