/*
** Alimer - Copyright (C) Amer Koleci
**
** This file is subject to the terms and conditions defined in
** file 'LICENSE.md', which is part of this source code package.
*/

#pragma once

#include "Prerequisites.h"
#include <vector>

namespace Alimer
{
	/// Parse arguments from the command line. First argument is by default assumed to be the executable name and is skipped.
	ALIMER_API const std::vector<String>& ParseArguments(const String& cmdLine, bool skipFirstArgument = true);
	/// Parse arguments from the command line.
	ALIMER_API const std::vector<String>& ParseArguments(const char* cmdLine);
	/// Parse arguments from a wide char command line.
	ALIMER_API const std::vector<String>& ParseArguments(const WString& cmdLine);
	/// Parse arguments from a wide char command line.
	ALIMER_API const std::vector<String>& ParseArguments(const wchar_t* cmdLine);
	/// Parse arguments from argc & argv.
	ALIMER_API const std::vector<String>& ParseArguments(int argc, char** argv);

	ALIMER_API const std::vector<String>& GetArguments();
}