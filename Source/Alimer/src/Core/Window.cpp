/*
** Alimer - Copyright (C) 2016-2017 Amer Koleci.
**
** This file is subject to the terms and conditions defined in
** file 'LICENSE.md', which is part of this source code package.
*/

#include "Core/Window.h"
#include "WindowImpl.h"

#if defined(ALIMER_WINDOWS) && !defined(ALIMER_WINMODERN)
#include "Windows\WindowsWindow.h"
typedef Alimer::priv::WindowsWindow PlatformWindowImpl;
#endif

namespace Alimer
{
	Window::Window(uint32_t width, uint32_t height, const String& title)
		: _impl(new PlatformWindowImpl(width, height, title))
	{

	}

	Window::~Window()
	{
		// Delete the window implementation
		delete _impl;
		_impl = nullptr;

	}

	WindowHandle Window::GetWindowHandle() const
	{
		return _impl ? _impl->GetWindowHandle() : 0;
	}
}