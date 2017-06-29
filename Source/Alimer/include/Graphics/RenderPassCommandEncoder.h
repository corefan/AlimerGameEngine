/*
** Alimer - Copyright (C) 2016-2017 Amer Koleci.
**
** This file is subject to the terms and conditions defined in
** file 'LICENSE.md', which is part of this source code package.
*/

#pragma once

#include "Core/RefPtr.h"
#include "Graphics/CommandEncoder.h"
#include "Math/Rect.h"

namespace Alimer
{
	/**
	* Defines a fixed-function viewport.
	*/
	struct Viewport {
		float x = 0.0f;
		float y = 0.0f;
		float width = 0.0f;
		float height = 0.0f;
		float minDepth = 0.0f;
		float maxDepth = 1.0f;

		Viewport() = default;
		Viewport(float x, float y, float width, float height)
			: x(x), y(y), width(width), height(height) {}

		explicit Viewport(IntRect rect)
			: x(static_cast<float>(rect.x)),
			y(static_cast<float>(rect.y)),
			width(static_cast<float>(rect.width)),
			height(static_cast<float>(rect.height)) {}
	};

	class ALIMER_API RenderPassCommandEncoder : public CommandEncoder {
	protected:
		explicit RenderPassCommandEncoder(CommandBuffer* commandBuffer)
			: CommandEncoder(commandBuffer) {}

	public:
		virtual void SetScissors(uint32_t scissorCount, const IntRect* pScissors) = 0;
		virtual void SetScissor(IntRect scissor) { SetScissors(1, &scissor); }

		virtual void SetViewports(uint32_t viewportCount, const Viewport* pViewports) = 0;
		virtual void SetViewport(Viewport viewport) { SetViewports(1, &viewport ); }
		void SetViewport(IntRect rect) { SetViewport(Viewport(rect)); }
	};

	using RenderPassCommandEncoderPtr = RefPtr<RenderPassCommandEncoder>;
}