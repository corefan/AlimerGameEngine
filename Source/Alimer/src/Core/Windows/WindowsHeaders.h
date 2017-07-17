/*
** Alimer - Copyright (C) Amer Koleci
**
** This file is subject to the terms and conditions defined in
** file 'LICENSE.md', which is part of this source code package.
*/

#pragma once

#include "Prerequisites.h"

#ifndef STRICT
#	define STRICT 1
#endif

#ifndef WIN32_LEAN_AND_MEAN
#	define WIN32_LEAN_AND_MEAN 1
#endif

#ifndef NOMINMAX
#	define NOMINMAX 1
#endif

//#define NODRAWTEXT
//#define NOGDI
//#define NOBITMAP
#define NOMCX
#define NOSERVICE
#define NOHELP

#include <windows.h>
#include <objbase.h>
#include <wrl.h>

#if defined(_XBOX_ONE) && defined(_TITLE)
#	include <xma2defs.h>
#	pragma comment(lib,"acphal.lib")
#endif

#if defined(WINAPI_FAMILY) && WINAPI_FAMILY == WINAPI_FAMILY_PHONE_APP
#	pragma comment(lib,"PhoneAudioSes.lib")
#endif

using Microsoft::WRL::ComPtr;

#if ALIMER_WINDOWS_FAMILY && !ALIMER_WINMODERN
#	define WINDOWS_USE_DYNAMIC_LIB 1
#endif

namespace Alimer
{
	// Helper class for COM exceptions
	class com_exception : public std::exception
	{
	public:
		com_exception(HRESULT hr) : result(hr) {}

		virtual const char* what() const override
		{
			static char s_str[64] = {};
			sprintf_s(s_str, "Failure with HRESULT of %08X", result);
			return s_str;
		}

	private:
		HRESULT result;
	};

	// Helper utility converts D3D API failures into exceptions.
	inline void ThrowIfFailed(HRESULT hr)
	{
		if (FAILED(hr))
		{
			throw com_exception(hr);
		}
	}

	inline ULONG GetRefCount(IUnknown* _interface)
	{
		_interface->AddRef();
		return _interface->Release();
	}

	struct handle_closer { void operator()(HANDLE h) { if (h) CloseHandle(h); } };
	typedef public std::unique_ptr<void, handle_closer> ScopedHandle;

	HRESULT AlimerCoInitialize();
}