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
	* Framebuffer.
	*/
	class ALIMER_API Framebuffer : public RefCounted
	{
	protected:
		/**
		* Constructor.
		*/
		Framebuffer();

	public:
		/**
		* Destructor.
		*/
		virtual ~Framebuffer();

	protected:
		DISALLOW_COPY_AND_ASSIGN(Framebuffer);
	};
}