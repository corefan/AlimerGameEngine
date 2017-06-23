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
		Window(uint32_t width, uint32_t height, const String& title, bool resizable = true, bool fullscreen = false);

		/**
		* Constructor.
		*/
		explicit Window(WindowHandle handle);

		/**
		* Destructor.
		*/
		virtual ~Window();

		/**
		* Get the OS-specific handle of the window.
		*/
		WindowHandle GetWindowHandle() const;

		/**
		* Get width of the window.
		*/
		uint32_t GetWidth() const {
			return _width;
		}

		/**
		* Get height of the window.
		*/
		uint32_t GetHeight() const {
			return _height;
		}

		/**
		* Get the window title.
		*/
		const String& GetTitle() const;

		/**
		* Set the window title.
		*/
		void SetTitle(const String& title);

	private:
		DISALLOW_COPY_AND_ASSIGN(Window);

		/// Platform-specific implementation of the window.
		priv::WindowImpl* _impl = nullptr;

		uint32_t _width;
		uint32_t _height;
	};
}