/*
** Alimer - Copyright (C) 2016-2017 Amer Koleci.
**
** This file is subject to the terms and conditions defined in
** file 'LICENSE.md', which is part of this source code package.
*/

#pragma once

#include "Prerequisites.h"

namespace Alimer
{
	/// Maximum number of Color attachments that can be bound simultaneously.
	static const uint32_t MAX_COLOR_ATTACHMENTS = 8;

	enum class LoadAction : uint8_t 
	{
		Load,
		Clear,
		DontCare
	};

	enum class StoreAction : uint8_t 
	{
		Store,
		DontCare
	};

	class Texture;

	struct RenderPassAttachment {
		Texture* texture = nullptr;
	};

	struct RenderPassColorAttachment : public RenderPassAttachment {
		LoadAction loadAction = LoadAction::DontCare;
		StoreAction storeAction = StoreAction::Store;
		//Color clearColor = Color(0.025f, 0.025f, 0.025f, 1.0f);
	};

	struct RenderPassDepthAttachment : public RenderPassAttachment {
		LoadAction loadAction = LoadAction::Clear;
		StoreAction storeAction = StoreAction::DontCare;

		float clearDepth = 1.0f;
		uint32_t clearStencil = 0;
	};

	struct RenderPassDescription
	{
		RenderPassColorAttachment colorAttachments[MAX_COLOR_ATTACHMENTS];
		RenderPassDepthAttachment depthStencilAttachment;
	};
}