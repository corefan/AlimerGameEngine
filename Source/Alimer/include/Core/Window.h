/*
** Alimer - Copyright (C) 2016-2017 Amer Koleci.
**
** This file is subject to the terms and conditions defined in
** file 'LICENSE.md', which is part of this source code package.
*/

#pragma once

#include "Prerequisites.h"
#include "Core/WindowHandle.h"

namespace Alimer
{
	namespace priv
	{
		class WindowImpl;
	}

	/**
	* Operating system window.
	*/
	class ALIMER_API Window
	{
	public:
		/**
		* Constructor.
		*/
		Window(uint32_t width, uint32_t height, const String& title);

		/**
		* Destructor.
		*/
		virtual ~Window();

		/**
		* Get the OS-specific handle of the window.
		*/
		WindowHandle GetWindowHandle() const;

	private:
		DISALLOW_COPY_AND_ASSIGN(Window);

		/// Platform-specific implementation of the window.
		priv::WindowImpl* _impl;           
	};
}