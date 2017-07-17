/*
** Alimer - Copyright (C) Amer Koleci
**
** This file is subject to the terms and conditions defined in
** file 'LICENSE.md', which is part of this source code package.
*/

#pragma once

#include "Core/Windows/WindowsHeaders.h"

#if	ALIMER_WINDOWS_FAMILY

#include <xaudio2.h>

typedef enum XAUDIO2_DEVICE_ROLE
{
	NotDefaultDevice = 0x0,
	DefaultConsoleDevice = 0x1,
	DefaultMultimediaDevice = 0x2,
	DefaultCommunicationsDevice = 0x4,
	DefaultGameDevice = 0x8,
	GlobalDefaultDevice = 0xf,
	InvalidDeviceRole = ~GlobalDefaultDevice
} XAUDIO2_DEVICE_ROLE;

// Returned by IXAudio2::GetDeviceDetails
typedef struct XAUDIO2_DEVICE_DETAILS
{
	WCHAR DeviceID[256];                // String identifier for the audio device.
	WCHAR DisplayName[256];             // Friendly name suitable for display to a human.
	XAUDIO2_DEVICE_ROLE Role;           // Roles that the device should be used for.
	WAVEFORMATEXTENSIBLE OutputFormat;  // The device's native PCM audio output format.
} XAUDIO2_DEVICE_DETAILS;

HRESULT XAudio27CreateProc(IXAudio2** ppXAudio2, UINT32 Flags, XAUDIO2_PROCESSOR XAudio2Processor);
HRESULT IXAudio2CreateMasteringVoice(IXAudio2* pXAudio2, UINT32 DeviceIndex, IXAudio2MasteringVoice** ppMasteringVoice);
HRESULT IXAudio2CreateSourceVoice(IXAudio2* pXAudio2, IXAudio2SourceVoice** ppSourceVoice, const WAVEFORMATEX* pSourceFormat);
HRESULT IXAudio27GetDeviceCount(IXAudio2* pXAudio2, __out UINT32* pCount);
HRESULT IXAudio27GetDeviceDetails(IXAudio2* pXAudio2, UINT32 Index, __out XAUDIO2_DEVICE_DETAILS* pDeviceDetails);
HRESULT IXAudio27StartEngine(IXAudio2* pXAudio2);
void IXAudio27StopEngine(IXAudio2* pXAudio2);

HRESULT IXAudio27RegisterForCallbacks(IXAudio2* pXAudio2, __in IXAudio2EngineCallback* pCallback);
void IXAudio27UnregisterForCallbacks(IXAudio2* pXAudio2, __in IXAudio2EngineCallback* pCallback);
#endif
