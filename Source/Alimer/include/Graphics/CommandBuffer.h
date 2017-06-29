/*
** Alimer - Copyright (C) 2016-2017 Amer Koleci.
**
** This file is subject to the terms and conditions defined in
** file 'LICENSE.md', which is part of this source code package.
*/

#pragma once

#include "Graphics/GraphicsResource.h"
#include "Graphics/CommandEncoder.h"
#include "Graphics/RenderPassCommandEncoder.h"

namespace Alimer
{
	/**
	* A bitmask indicating the kind of queue operations will require.
	*/
	enum class CommandQueueMask : uint32_t
	{
		None = 0,
		// A queue supporting render operations (such as Draw*).
		Render = 1 << 0,
		// A queue supporting compute operations (such as Dispatch*).
		Compute = 1 << 1,
		// A queue supporting transfer operations (such as CopyBuffer).
		Transfer = 1 << 2,
	};

	ALIMER_BITMASK(CommandQueueMask);

	class GraphicsDevice;

	/**
	* Command Buffer.
	*/
	class ALIMER_API CommandBuffer : public GraphicsResource
	{
	protected:
		/**
		* Constructor.
		*/
		CommandBuffer(GraphicsDevice* device);

	public:
		/**
		* Destructor.
		*/
		virtual ~CommandBuffer();

		RenderPassCommandEncoderPtr CreateRenderCommandEncoder(const RenderPassDescription& desc);

		// A bitmask indicating on which queue types this command buffer will execute
		// based on the commands that were encoded into it.
		CommandQueueMask GetQueueMask() const { return _queueMask; }

	protected:
		virtual RenderPassCommandEncoderPtr OnCreateRenderCommandEncoder(const RenderPassDescription& desc) = 0;

		CommandQueueMask _queueMask = CommandQueueMask::None;

	private:
		DISALLOW_COPY_AND_ASSIGN(CommandBuffer);
	};
}