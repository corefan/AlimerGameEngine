/*
** Alimer - Copyright (C) 2016-2017 Amer Koleci.
**
** This file is subject to the terms and conditions defined in
** file 'LICENSE.md', which is part of this source code package.
*/

#include "Graphics/GraphicsResource.h"
#include "Graphics/GraphicsDevice.h"

namespace Alimer
{
	GraphicsResource::GraphicsResource(GraphicsDevice* device, GraphicsResourceType resourceType)
		: _graphics(device)
		, _resourceType(resourceType)
	{
		ALIMER_ASSERT(_graphics);
		_graphics->AddGraphicsResource(this);
	}

	GraphicsResource::~GraphicsResource()
	{
		if (!_graphics.IsNull() && !_graphics.IsExpired())
		{
			_graphics->RemoveGraphicsResource(this);
		}
	}

	void GraphicsResource::Release()
	{
	}

	GraphicsDevice* GraphicsResource::GetDevice() const
	{
		return _graphics.Get();
	}

	GraphicsResourceType GraphicsResource::GetResourceType() const
	{
		return _resourceType;
	}
}