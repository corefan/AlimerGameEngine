/*
** Alimer - Copyright (C) 2016-2017 Amer Koleci.
**
** This file is subject to the terms and conditions defined in
** file 'LICENSE.md', which is part of this source code package.
*/

#pragma once

#include "Core/RefPtr.h"
#include "Core/WeakPtr.h"

namespace Alimer
{
	class GraphicsDevice;

	enum class GraphicsResourceType
	{
		CommandBuffer,
		SwapChain,
		Texture,
		Shader
	};

	/**
	* GraphicsResource abstract class.
	*/
	class ALIMER_API GraphicsResource : public RefCounted
	{
		friend class GraphicsDevice;

	protected:
		/**
		* Constructor.
		*/
		GraphicsResource(GraphicsDevice* device, GraphicsResourceType resourceType);

	public:
		/**
		* Destructor.
		*/
		virtual ~GraphicsResource();

		/// Gets the GraphicsDevice associated with this Graphics resource.
		GraphicsDevice* GetDevice() const;

		/// Gets the type of Graphics resource.
		GraphicsResourceType GetResourceType() const;

	protected:
		/// Unconditionally release the GPU resource.
		virtual void Release();

		WeakPtr<GraphicsDevice> _graphics;
		GraphicsResourceType _resourceType;

	private:
		DISALLOW_COPY_AND_ASSIGN(GraphicsResource);
	};
}