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
	* Texture.
	*/
	class ALIMER_API Texture : public RefCounted
	{
	protected:
		/**
		* Constructor.
		*/
		Texture();

	public:
		/**
		* Destructor.
		*/
		virtual ~Texture();

	protected:
		DISALLOW_COPY_AND_ASSIGN(Texture);
	};

	using TexturePtr = RefPtr<Texture>;
}