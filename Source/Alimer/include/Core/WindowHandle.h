/*
** Alimer - Copyright (C) 2016-2017 Amer Koleci.
**
** This file is subject to the terms and conditions defined in
** file 'LICENSE.md', which is part of this source code package.
*/

#pragma once

#include "Platform.h"

// Windows' HWND is a typedef on struct HWND__*
#if ALIMER_WINDOWS_FAMILY && !ALIMER_WINMODERN
struct HWND__;
#endif

namespace Alimer
{
#if ALIMER_WINDOWS_FAMILY && !ALIMER_WINMODERN
	// Window handle is HWND (HWND__*) on Windows
	typedef HWND__* WindowHandle;

#elif ALIMER_LINUX

	// window handle is window (unsigned long) on Unix - X11
	typedef unsigned long WindowHandle;

#else

	// Window handle is NSWindow or NSView (void*) on Mac OS X - Cocoa
	// Window handle is UIWindow (void*) on iOS - UIKit
	// Window handle is ANativeWindow* (void*) on Android
	typedef void* WindowHandle;

#endif
}