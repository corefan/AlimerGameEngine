/*
** Alimer - Copyright (C) 2016-2017 Amer Koleci.
**
** This file is subject to the terms and conditions defined in
** file 'LICENSE.md', which is part of this source code package.
*/

#include "Graphics/SwapChain.h"

namespace Alimer
{
	SwapChain::SwapChain(GraphicsDevice* device)
		: GraphicsResource(device, GraphicsResourceType::SwapChain)
	{

	}

	SwapChain::~SwapChain()
	{
	}
}