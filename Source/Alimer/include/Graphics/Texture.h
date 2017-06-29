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
	class GraphicsDevice;

	/**
	* Texture.
	*/
	class ALIMER_API Texture : public GraphicsResource
	{
	protected:
		/**
		* Constructor.
		*/
		Texture(GraphicsDevice* device);

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