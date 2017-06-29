/*
** Alimer - Copyright (C) 2016-2017 Amer Koleci.
**
** This file is subject to the terms and conditions defined in
** file 'LICENSE.md', which is part of this source code package.
*/

#include "Graphics/CommandBuffer.h"

namespace Alimer
{
	CommandBuffer::CommandBuffer(GraphicsDevice* device)
		: GraphicsResource(device, GraphicsResourceType::CommandBuffer)
	{
		
	}

	CommandBuffer::~CommandBuffer()
	{
	}

	RenderPassCommandEncoderPtr CommandBuffer::CreateRenderCommandEncoder(const RenderPassDescription& desc)
	{
		_queueMask |= CommandQueueMask::Render;
		return OnCreateRenderCommandEncoder(desc);
	}
}