/*
** Alimer - Copyright (C) 2016-2017 Amer Koleci.
**
** This file is subject to the terms and conditions defined in
** file 'LICENSE.md', which is part of this source code package.
*/

#pragma once

#include "Graphics/Framebuffer.h"
#include "Core/Window.h"

namespace Alimer
{
	/**
	* SwapChain.
	*/
	class ALIMER_API SwapChain : public RefCounted
	{
	protected:
		/**
		* Constructor.
		*/
		SwapChain();

	public:
		/**
		* Destructor.
		*/
		virtual ~SwapChain();

		virtual void Present() = 0;

		uint32_t GetCurrentBackBufferIndex() const {
			return _currentBackBufferIndex;
		}

		RefPtr<Framebuffer> GetCurrentBackBuffer() const {
			return _backBufferFramebuffers[_currentBackBufferIndex];
		}

		uint32_t GetFramebufferCount() const {
			return (uint32_t)_backBufferFramebuffers.size();
		}

	protected:
		DISALLOW_COPY_AND_ASSIGN(SwapChain);

		uint32_t _currentBackBufferIndex = 0;
		std::vector<RefPtr<Framebuffer> > _backBufferFramebuffers;
	};
}