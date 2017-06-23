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
#elif defined(ALIMER_ANDROID)
#include "Android\AndroidWindow.h"
typedef Alimer::priv::AndroidWindow PlatformWindowImpl;
#elif defined(ALIMER_HTML5)
#include "HTML5\WebWindow.h"
typedef Alimer::priv::WebWindow PlatformWindowImpl;
#endif

namespace Alimer
{
	Window::Window(uint32_t width, uint32_t height, const String& title, bool resizable, bool fullscreen)
		: _impl(new PlatformWindowImpl(width, height, title, resizable, fullscreen))
	{
		_impl->GetSize(&_width, &_height);
	}

	Window::Window(WindowHandle handle)
		: _impl(new PlatformWindowImpl(handle))
	{
		_impl->GetSize(&_width, &_height);
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

	const String& Window::GetTitle() const
	{
		return _impl->GetTitle();
	}

	void Window::SetTitle(const String& title)
	{
		_impl->SetTitle(title);
	}
}