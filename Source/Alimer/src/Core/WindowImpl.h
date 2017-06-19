/*
** Alimer - Copyright (C) 2016-2017 Amer Koleci.
**
** This file is subject to the terms and conditions defined in
** file 'LICENSE.md', which is part of this source code package.
*/

#pragma once

#include "Prerequisites.h"
#include "Core/WindowHandle.h"

namespace Alimer
{
	namespace priv
	{
		class WindowImpl
		{
		protected:
			WindowImpl() {}
			 
		public:
			virtual ~WindowImpl() {}

			virtual WindowHandle GetWindowHandle() const = 0;

		private:
			DISALLOW_COPY_AND_ASSIGN(WindowImpl);
		};
	}
}