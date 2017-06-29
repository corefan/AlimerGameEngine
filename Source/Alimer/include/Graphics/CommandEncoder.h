/*
** Alimer - Copyright (C) 2016-2017 Amer Koleci.
**
** This file is subject to the terms and conditions defined in
** file 'LICENSE.md', which is part of this source code package.
*/

#pragma once

#include "Graphics/Types.h"
#include "Core/RefCounted.h"

namespace Alimer
{
	class CommandBuffer;

	/**
	* Base class for command buffer encoders.
	*/
	class ALIMER_API CommandEncoder : public RefCounted
	{
	public:
		/**
		* Destructor.
		*/
		virtual ~CommandEncoder() { }

		/**
		* 
		*/
		virtual void EndEncoding() = 0;

	protected:
		DISALLOW_COPY_AND_ASSIGN(CommandEncoder);

		/**
		* Constructor.
		*/
		explicit CommandEncoder(CommandBuffer* commandBuffer)
			: _commandBuffer(commandBuffer) {}

		CommandBuffer* _commandBuffer;
	};
}