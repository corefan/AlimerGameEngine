/*
** Alimer - Copyright (C) 2016-2017 Amer Koleci.
**
** This file is subject to the terms and conditions defined in
** file 'LICENSE.md', which is part of this source code package.
*/

#pragma once

#include "Core/RefCounted.h"

namespace Alimer
{
	/**
	* Command Buffer.
	*/
	class ALIMER_API CommandBuffer : public RefCounted
	{
	protected:
		/**
		* Constructor.
		*/
		CommandBuffer();

	public:
		/**
		* Destructor.
		*/
		virtual ~CommandBuffer();

	protected:
		DISALLOW_COPY_AND_ASSIGN(CommandBuffer);
	};
}