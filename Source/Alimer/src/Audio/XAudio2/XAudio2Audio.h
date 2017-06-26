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
		void Pause() override;
		void Resume() override;

	private:
		struct AudioEngineCallback : public IXAudio2EngineCallback
		{
			AudioEngineCallback()
			{
				criticalError.reset(CreateEventEx(nullptr, nullptr, 0, EVENT_MODIFY_STATE | SYNCHRONIZE));
				if (!criticalError)
				{
					throw std::exception("CreateEvent");
				}
			};

			virtual ~AudioEngineCallback()
			{
			}

			STDMETHOD_(void, OnProcessingPassStart) () override {}
			STDMETHOD_(void, OnProcessingPassEnd)() override {}

			STDMETHOD_(void, OnCriticalError) (THIS_ HRESULT error)
			{
#ifndef _DEBUG
				UNREFERENCED_PARAMETER(error);
#endif
				
				ALIMER_LOGERROR("XAudio2 engine encountered critical error (%08X)\n", error);
				SetEvent(criticalError.get());
			}

			ScopedHandle criticalError;
		};

	private:
		ComPtr<IXAudio2> xAudio2;
		IXAudio2MasteringVoice* _masterVoice = nullptr;
		IXAudio2SubmixVoice* _reverbVoice = nullptr;
		X3DAUDIO_HANDLE  _X3DAudio;

		uint32_t _masterChannelMask = 0;
		uint32_t _masterChannels = 0;
		uint32_t _masterRate = 0;
		bool _criticalError = false;

		AudioEngineCallback _engineCallback;

#if WINDOWS_USE_DYNAMIC_LIB
		HMODULE _xAudioDLL = nullptr;
		HMODULE _x3DAudioDLL = nullptr;
		bool _usingXAudio27 = false;
#endif
	};
}