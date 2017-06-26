/*
** Alimer - Copyright (C) Amer Koleci
**
** This file is subject to the terms and conditions defined in
** file 'LICENSE.md', which is part of this source code package.
*/

#pragma once

#include "XAudio2Prerequisites.h"
#include "Audio/Audio.h"

namespace Alimer
{
	class XAudio2Audio final : public Audio
	{
	public:
		XAudio2Audio();

		virtual ~XAudio2Audio();

		bool Initialize() override;

	private:
		ComPtr<IXAudio2> xAudio2;
		IXAudio2MasteringVoice* _masterVoice = nullptr;
		IXAudio2SubmixVoice* _reverbVoice = nullptr;
		X3DAUDIO_HANDLE  _X3DAudio;

		uint32_t _masterChannelMask = 0;
		uint32_t _masterChannels = 0;
		uint32_t _masterRate = 0;

#if WINDOWS_USE_DYNAMIC_LIB
		HMODULE _xAudioDLL = nullptr;
		HMODULE _x3DAudioDLL = nullptr;
		bool _usingXAudio27 = false;
#endif
	};
}