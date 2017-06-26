/*
** Alimer - Copyright (C) Amer Koleci
**
** This file is subject to the terms and conditions defined in
** file 'LICENSE.md', which is part of this source code package.
*/

#include "XAudio2Audio.h"

#if defined(WINDOWS_USE_DYNAMIC_LIB)
#	include "XAudio27.h"

typedef HRESULT(__stdcall* PFN_XAudioCreate)(_Outptr_ IXAudio2** ppXAudio2, UINT32 Flags, XAUDIO2_PROCESSOR XAudio2Processor);
typedef HRESULT(_cdecl * PFN_X3DAudioInitialize)(UINT32 SpeakerChannelMask, FLOAT32 SpeedOfSound, _Out_writes_bytes_(X3DAUDIO_HANDLE_BYTESIZE) X3DAUDIO_HANDLE Instance);
typedef void(_cdecl * PFN_X3DAudioCalculate)(_In_reads_bytes_(X3DAUDIO_HANDLE_BYTESIZE) const X3DAUDIO_HANDLE Instance, _In_ const X3DAUDIO_LISTENER* pListener, _In_ const X3DAUDIO_EMITTER* pEmitter, UINT32 Flags, _Inout_ X3DAUDIO_DSP_SETTINGS* pDSPSettings);
PFN_X3DAudioInitialize X3DAudioInitializeFunc;
PFN_X3DAudioCalculate X3DAudioCalculateFunc;
#endif

#define SAFE_DESTROY_VOICE(voice) if ( voice ) { voice->DestroyVoice(); voice = nullptr; }

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
		memset(&_X3DAudio, 0, X3DAUDIO_HANDLE_BYTESIZE);
	}

	XAudio2Audio::~XAudio2Audio()
	{
		if (xAudio2)
		{
			if (_usingXAudio27)
			{
				IXAudio27StopEngine(xAudio2.Get());
			}
			else
			{
				//xAudio2->UnregisterForCallbacks(&_engineCallback);
				xAudio2->StopEngine();
			}
		}

		SAFE_DESTROY_VOICE(_reverbVoice);
		SAFE_DESTROY_VOICE(_masterVoice);
		_masterChannelMask = _masterChannels = _masterRate = 0;
		//_criticalError = false;

		xAudio2.Reset();

		// Shutdown Media Foundation.
		MFShutdown();

		memset(&_X3DAudio, 0, X3DAUDIO_HANDLE_BYTESIZE);

#if WINDOWS_USE_DYNAMIC_LIB
		if (_xAudioDLL)
		{
			FreeLibrary(_xAudioDLL);
			_xAudioDLL = nullptr;
		}

		if (_x3DAudioDLL)
		{
			FreeLibrary(_x3DAudioDLL);
			_x3DAudioDLL = nullptr;
		}
#endif
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