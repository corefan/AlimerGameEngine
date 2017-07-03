/*
** Alimer - Copyright (C) 2016-2017 Amer Koleci.
**
** This file is subject to the terms and conditions defined in
** file 'LICENSE.md', which is part of this source code package.
*/

#pragma once

#include "Graphics/GraphicsResource.h"

namespace Alimer
{
	enum class ShaderStage
	{
		/**
		* Vertex shader stage.
		*/
		Vertex = 0,
		/**
		* Tessellation control shader stage (hull).
		*/
		TessellationControl = 1,
		/**
		* Tessellation control shader stage (domain).
		*/
		TessellationEvaluation = 2,
		/**
		* Geometry shader stage.
		*/
		Geometry = 3,
		/**
		* Fragment shader (pixel).
		*/
		Fragment = 4,
		/**
		* Compute shader stage.
		*/
		Compute = 5,
		/**
		* Count of shader types.
		*/
		Count
	};

	class GraphicsDevice;

	/**
	* Shader.
	*/
	class ALIMER_API Shader : public GraphicsResource
	{
	protected:
		/**
		* Constructor.
		*/
		Shader(GraphicsDevice* device, ShaderStage stage, uint32_t codeSize, const uint32_t* code);

	public:
		/**
		* Destructor.
		*/
		virtual ~Shader();

		/**
		* Gets the Shader stage.
		*/
		ShaderStage GetStage() const { return _stage; }
		
	protected:
		ShaderStage _stage;
		std::vector<uint32_t> spirv;

	private:
		DISALLOW_COPY_AND_ASSIGN(Shader);
	};

	using ShaderPtr = RefPtr<Shader>;
}