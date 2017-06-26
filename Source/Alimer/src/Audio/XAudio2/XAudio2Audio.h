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
	};
}