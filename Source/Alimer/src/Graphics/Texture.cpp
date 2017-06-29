/*
** Alimer - Copyright (C) 2016-2017 Amer Koleci.
**
** This file is subject to the terms and conditions defined in
** file 'LICENSE.md', which is part of this source code package.
*/

#include "Graphics/Texture.h"

namespace Alimer
{
	Texture::Texture(GraphicsDevice* device)
		: GraphicsResource(device, GraphicsResourceType::Texture)
	{

	}

	Texture::~Texture()
	{
	}
}