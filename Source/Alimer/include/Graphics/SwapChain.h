/*
** Alimer - Copyright (C) 2016-2017 Amer Koleci.
**
** This file is subject to the terms and conditions defined in
** file 'LICENSE.md', which is part of this source code package.
*/

#pragma once

#include "Graphics/GraphicsResource.h"
#include "Graphics/Texture.h"
#include "Core/Window.h"

namespace Alimer
{
	class GraphicsDevice;

	/**
	* SwapChain.
	*/
	class ALIMER_API SwapChain : public GraphicsResource
	{
	protected:
		/**
		* Constructor.
		*/
		SwapChain(GraphicsDevice* device);

	public:
		/**
		* Destructor.
		*/
		virtual ~SwapChain();

		virtual void Present() = 0;

		uint32_t GetCurrentBackBufferIndex() const {
			return _currentBackBufferIndex;
		}

		TexturePtr GetCurrentBackBuffer() const {
			return _backbufferTextures[_currentBackBufferIndex];
		}

		uint32_t GetBackBuffersCount() const {
			return (uint32_t)_backbufferTextures.size();
		}


	protected:
		uint32_t _currentBackBufferIndex = 0;
		std::vector<TexturePtr> _backbufferTextures;

	private:
		DISALLOW_COPY_AND_ASSIGN(SwapChain);
	};
}