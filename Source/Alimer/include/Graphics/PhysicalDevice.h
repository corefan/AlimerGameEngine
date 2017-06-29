/*
** Alimer - Copyright (C) 2016-2017 Amer Koleci.
**
** This file is subject to the terms and conditions defined in
** file 'LICENSE.md', which is part of this source code package.
*/

#pragma once

#include "Core/RefCounted.h"

namespace Alimer
{
	class ALIMER_API PhysicalDevice : public RefCounted
	{
	protected:
		PhysicalDevice();

	public:
		virtual ~PhysicalDevice();

	protected:

	private:
		DISALLOW_COPY_AND_ASSIGN(PhysicalDevice);
	};
}