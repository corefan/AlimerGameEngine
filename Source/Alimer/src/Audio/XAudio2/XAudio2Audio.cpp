/*
** Alimer - Copyright (C) Amer Koleci
**
** This file is subject to the terms and conditions defined in
** file 'LICENSE.md', which is part of this source code package.
*/

#include "XAudio2Audio.h"

bool LoadModuleWithErrorMessage(const char* moduleName)
{
	if (!LoadLibraryA(moduleName))
	{
		uint32 errorCode = GetLastError();
		if (errorCode == ERROR_MOD_NOT_FOUND)
		{
			ALIMER_LOGERROR("XAudio: Could not load '%s'.", moduleName);
		}
		else
		{
			ALIMER_LOGERROR("XAudio: Could not load '%s', error code: %d", moduleName, errorCode);
		}

		return false;
	}

	return true;
}

namespace Alimer
{
	XAudio2Audio::XAudio2Audio()
		: Audio(AudioDeviceType::XAudio2)
	{

	}

	XAudio2Audio::~XAudio2Audio()
	{
		// Shutdown Media Foundation.
		MFShutdown();
	}

	bool XAudio2Audio::Initialize()
	{
		if (!Audio::Initialize())
			return false;

		HRESULT hr = S_OK;

#if	defined(WINDOWS_USE_DYNAMIC_LIB)
		hr = AlimerCoInitialize();
		if (FAILED(hr))
		{
			ALIMER_LOGERROR("Failed to initialize COM");
			return false;
		}

		// Load MediaFoundation libraries.
		if (!LoadModuleWithErrorMessage("shlwapi.dll"))
		{
			return false;
		}

		if (!LoadModuleWithErrorMessage("mf.dll"))
		{
			return false;
		}

		if (!LoadModuleWithErrorMessage("mfplat.dll"))
		{
			return false;
		}

		if (!LoadModuleWithErrorMessage("mfplay.dll"))
		{
			return false;
		}
#endif

		// Initialize Media Foundation.
		hr = MFStartup(MF_VERSION);
		if (FAILED(hr))
		{
			ALIMER_LOGERROR("Failed to initialize Media Foundation, Error: %i", hr);
		}

		return true;
	}
}