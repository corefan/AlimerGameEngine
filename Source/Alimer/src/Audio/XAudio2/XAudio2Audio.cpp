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
typedef void(_cdecl * PFN_X3DAudioInitialize)(UINT32 SpeakerChannelMask, FLOAT32 SpeedOfSound, _Out_writes_bytes_(X3DAUDIO_HANDLE_BYTESIZE) X3DAUDIO_HANDLE Instance);
typedef HRESULT(_cdecl * PFN_X3DAudioInitializeWithHR)(UINT32 SpeakerChannelMask, FLOAT32 SpeedOfSound, _Out_writes_bytes_(X3DAUDIO_HANDLE_BYTESIZE) X3DAUDIO_HANDLE Instance);
typedef void(_cdecl * PFN_X3DAudioCalculate)(_In_reads_bytes_(X3DAUDIO_HANDLE_BYTESIZE) const X3DAUDIO_HANDLE Instance, _In_ const X3DAUDIO_LISTENER* pListener, _In_ const X3DAUDIO_EMITTER* pEmitter, UINT32 Flags, _Inout_ X3DAUDIO_DSP_SETTINGS* pDSPSettings);

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
			ALIMER_LOGERRORF("XAudio: Could not load '%s'.", moduleName);
		}
		else
		{
			ALIMER_LOGERRORF("XAudio: Could not load '%s', error code: %d", moduleName, errorCode);
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
				IXAudio27UnregisterForCallbacks(xAudio2.Get(), &_engineCallback);
				IXAudio27StopEngine(xAudio2.Get());
			}
			else
			{
				xAudio2->UnregisterForCallbacks(&_engineCallback);
				xAudio2->StopEngine();
			}
		}

		SAFE_DESTROY_VOICE(_reverbVoice);
		SAFE_DESTROY_VOICE(_masterVoice);
		_masterChannelMask = _masterChannels = _masterRate = 0;
		_criticalError = false;

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
			ALIMER_LOGERRORF("Failed to initialize Media Foundation, Error: %i", hr);
		}

		// Create XAudio2 engine
		UINT32 eflags = 0;
#if WINDOWS_USE_DYNAMIC_LIB
		uint16_t apiMinorVersion = 9;
		_xAudioDLL = LoadLibraryExW(L"XAudio2_9", nullptr, LOAD_LIBRARY_SEARCH_SYSTEM32);

		if (!_xAudioDLL)
		{
			_xAudioDLL = LoadLibraryExW(L"XAudio2_8", nullptr, LOAD_LIBRARY_SEARCH_SYSTEM32);
			apiMinorVersion = 8;
		}

		const bool forceUseXAudio27 = false;
		const bool enableDebug = false;
		if (!_xAudioDLL || forceUseXAudio27)
		{
			// Free old library if we are forcing version 2.7
			if (_xAudioDLL)
			{
				FreeLibrary(_xAudioDLL);
				_xAudioDLL = nullptr;
			}

			// Windows 7
			apiMinorVersion = 7;
			_usingXAudio27 = true;
			_xAudioDLL = enableDebug ? LoadLibraryEx(L"XAudioD2_7.DLL", nullptr, 0x00000800 /* LOAD_LIBRARY_SEARCH_SYSTEM32 */) : LoadLibraryExW(L"XAudio2_7.DLL", nullptr, 0x00000800 /* LOAD_LIBRARY_SEARCH_SYSTEM32 */);
			_x3DAudioDLL = LoadLibraryExW(L"X3DAudio1_7", nullptr, LOAD_LIBRARY_SEARCH_SYSTEM32);

			if (!_xAudioDLL)
			{
				ALIMER_LOGERROR("ERROR: XAudio 2.7 not installed on system (install the DirectX End-user Runtimes (June 2010))");
				return false;
			}
		}

		if (_usingXAudio27)
		{
			hr = XAudio27CreateProc(xAudio2.ReleaseAndGetAddressOf(), eflags, XAUDIO2_DEFAULT_PROCESSOR);
		}
		else
		{
			_x3DAudioDLL = _xAudioDLL;

			PFN_XAudioCreate XAudio2CreateFunc = reinterpret_cast<PFN_XAudioCreate>(GetProcAddress(_xAudioDLL, "XAudio2Create"));
			if (!XAudio2CreateFunc)
				return false;

			hr = XAudio2CreateFunc(xAudio2.ReleaseAndGetAddressOf(), 0, XAUDIO2_DEFAULT_PROCESSOR);
		}

		X3DAudioCalculateFunc = (PFN_X3DAudioCalculate)GetProcAddress(_x3DAudioDLL, "X3DAudioCalculate");
		if (!X3DAudioCalculateFunc)
			return false;

#else
		hr = XAudio2Create(&xaudio2, eflags);
#endif

		if (FAILED(hr))
		{
			ALIMER_LOGERRORF("Failed to initialize XAudio2.%d, error: %i", apiMinorVersion, hr);
			return false;
		}

		if (enableDebug)
		{
			XAUDIO2_DEBUG_CONFIGURATION debug = {};
			debug.TraceMask = XAUDIO2_LOG_ERRORS | XAUDIO2_LOG_WARNINGS;
			debug.BreakMask = XAUDIO2_LOG_ERRORS;
			xAudio2->SetDebugConfiguration(&debug, nullptr);
			if (apiMinorVersion == 9)
			{
				ALIMER_LOGINFO("XAudio 2.9 debugging enabled");
			}
			else if (apiMinorVersion == 8)
			{
				// To see the trace output, you need to view ETW logs for this application:
				//    Go to Control Panel, Administrative Tools, Event Viewer.
				//    View->Show Analytic and Debug Logs.
				//    Applications and Services Logs / Microsoft / Windows / XAudio2. 
				//    Right click on Microsoft Windows XAudio2 debug logging, Properties, then Enable Logging, and hit OK 
				ALIMER_LOGINFO("XAudio 2.8 debugging enabled");
			}
			else
			{
				// To see the trace output, see the debug output channel window
				ALIMER_LOGINFO("XAudio 2.7 debugging enabled");
			}
		}

#if WINDOWS_USE_DYNAMIC_LIB
		if (_usingXAudio27)
			hr = IXAudio27RegisterForCallbacks(xAudio2.Get(), &_engineCallback);
		else
			hr = xAudio2->RegisterForCallbacks(&_engineCallback);
#else
		hr = xAudio2->RegisterForCallbacks(&_engineCallback);
#endif
		if (FAILED(hr))
		{
			xAudio2.Reset();
			return hr;
		}

		const wchar_t* deviceId = nullptr;
		if (!_usingXAudio27)
		{
			const AUDIO_STREAM_CATEGORY category = AudioCategory_GameEffects;
			hr = xAudio2->CreateMasteringVoice(&_masterVoice, XAUDIO2_DEFAULT_CHANNELS, XAUDIO2_DEFAULT_SAMPLERATE,
				0, deviceId, nullptr, category);

			if (FAILED(hr))
			{
				xAudio2.Reset();
				return hr;
			}

			DWORD dwChannelMask;
			hr = _masterVoice->GetChannelMask(&dwChannelMask);
			if (FAILED(hr))
			{
				SAFE_DESTROY_VOICE(_masterVoice);
				xAudio2.Reset();
				return hr;
			}

			XAUDIO2_VOICE_DETAILS details;
			_masterVoice->GetVoiceDetails(&details);

			_masterChannelMask = dwChannelMask;
			_masterChannels = details.InputChannels;
			_masterRate = details.InputSampleRate;

			// StartEngine
			hr = xAudio2->StartEngine();
			if (FAILED(hr))
			{
				xAudio2.Reset();
				return hr;
			}
		}
		else
		{
			UINT32 count = 0;
			hr = IXAudio27GetDeviceCount(xAudio2.Get(), &count);
			if (FAILED(hr))
			{
				xAudio2.Reset();
				return hr;
			}

			if (!count)
			{
				xAudio2.Reset();
				return HRESULT_FROM_WIN32(ERROR_NOT_FOUND);
			}

			UINT32 devIndex = 0;
			if (deviceId)
			{
				// Translate device ID back into device index
				devIndex = UINT32(-1);
				for (UINT32 j = 0; j < count; ++j)
				{
					XAUDIO2_DEVICE_DETAILS details;
					hr = IXAudio27GetDeviceDetails(xAudio2.Get(), j, &details);
					if (SUCCEEDED(hr))
					{
						if (wcsncmp(deviceId, details.DeviceID, 256) == 0)
						{
							devIndex = j;
							_masterChannelMask = details.OutputFormat.dwChannelMask;
							break;
						}
					}
				}

				if (devIndex == UINT32(-1))
				{
					xAudio2.Reset();
					return HRESULT_FROM_WIN32(ERROR_NOT_FOUND);
				}
			}
			else
			{
				// No search needed
				XAUDIO2_DEVICE_DETAILS details;
				hr = IXAudio27GetDeviceDetails(xAudio2.Get(), 0, &details);
				if (FAILED(hr))
				{
					xAudio2.Reset();
					return hr;
				}

				_masterChannelMask = details.OutputFormat.dwChannelMask;
			}

			hr = IXAudio2CreateMasteringVoice(xAudio2.Get(), devIndex, &_masterVoice);
			if (FAILED(hr))
			{
				xAudio2.Reset();
				return hr;
			}

			XAUDIO2_VOICE_DETAILS details;
			_masterVoice->GetVoiceDetails(&details);

			_masterChannels = details.InputChannels;
			_masterRate = details.InputSampleRate;

			// StartEngine
			hr = IXAudio27StartEngine(xAudio2.Get());
			if (FAILED(hr))
			{
				xAudio2.Reset();
				return hr;
			}
		}

		ALIMER_LOGDEBUGF("Mastering voice has %u channels, %u sample rate, %08X channel mask", _masterChannels, _masterRate, _masterChannelMask);

		// Setup 3D audio
		const float SPEEDOFSOUND = X3DAUDIO_SPEED_OF_SOUND;

#if WINDOWS_USE_DYNAMIC_LIB
		if (_usingXAudio27)
		{
			PFN_X3DAudioInitialize X3DAudioInitializeFunc = (PFN_X3DAudioInitialize)GetProcAddress(_x3DAudioDLL, "X3DAudioInitialize");
			if (!X3DAudioInitializeFunc)
				return S_FALSE;

			X3DAudioInitializeFunc(_masterChannelMask, SPEEDOFSOUND, _X3DAudio);
			hr = S_OK;
		}
		else
		{
			PFN_X3DAudioInitializeWithHR X3DAudioInitializeFunc = (PFN_X3DAudioInitializeWithHR)GetProcAddress(_x3DAudioDLL, "X3DAudioInitialize");
			if (!X3DAudioInitializeFunc)
				return S_FALSE;

			hr = X3DAudioInitializeFunc(_masterChannelMask, SPEEDOFSOUND, _X3DAudio);
		}
#else
		hr = X3DAudioInitialize(_masterChannelMask, SPEEDOFSOUND, _X3DAudio);
#endif

		if (FAILED(hr))
		{
			SAFE_DESTROY_VOICE(_reverbVoice);
			SAFE_DESTROY_VOICE(_masterVoice);
			//_reverbEffect.Reset();
			//_volumeLimiter.Reset();
			xAudio2.Reset();
			return false;
		}

		return true;
	}

	void XAudio2Audio::Pause()
	{
		if (!xAudio2)
			return;

		if (_usingXAudio27)
		{
			IXAudio27StopEngine(xAudio2.Get());
		}
		else
		{
			xAudio2->StopEngine();
		}

		Audio::Pause();
	}

	void XAudio2Audio::Resume()
	{
		if (!xAudio2)
			return;

		if (_usingXAudio27)
		{
			ThrowIfFailed(
				IXAudio27StartEngine(xAudio2.Get())
				);
		}
		else
		{
			ThrowIfFailed(xAudio2->StartEngine());
		}

		Audio::Resume();
	}
}