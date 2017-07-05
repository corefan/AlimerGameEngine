/*
** Alimer - Copyright (C) Amer Koleci
**
** This file is subject to the terms and conditions defined in
** file 'LICENSE.txt', which is part of this source code package.
*/

#pragma once

#include "Prerequisites.h"

namespace Alimer
{
	/**
	* File System class.
	*/
	class ALIMER_API FileSystem final 
	{
	public:
		/// Return the program's directory.
		static String GetProgramDir();

		/// Return the absolute current working directory.
		static String GetCurrentDir();

		/// Set the current working directory.
		static bool SetCurrentDir(const String& pathName);

	private:
		FileSystem() = delete;
		~FileSystem() = delete;

		DISALLOW_COPY_AND_ASSIGN(FileSystem);
	};
}