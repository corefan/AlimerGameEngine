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
			virtual void GetSize(uint32_t* width, uint32_t* height) const = 0;

			/**
			* Get the title  of the window.
			*/
			const String& GetTitle() const { return _title; }

			virtual void SetTitle(const String& title)
			{
				_title = title;
			}

		protected:
			String _title;

		private:
			DISALLOW_COPY_AND_ASSIGN(WindowImpl);
		};
	}
}