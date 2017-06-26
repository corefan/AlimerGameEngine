/*
** Alimer - Copyright (C) Amer Koleci
**
** This file is subject to the terms and conditions defined in
** file 'LICENSE.md', which is part of this source code package.
*/

#include "Utils/CommandLine.h"
#include "Utils/StringUtils.h"

namespace Alimer
{
	static std::vector<String> _arguments;

	const std::vector<String>& ParseArguments(const String& cmdLine, bool skipFirstArgument)
	{
		_arguments.clear();

		size_t cmdStart = 0, cmdEnd = 0;
		bool inCmd = false;
		bool inQuote = false;

		for (size_t i = 0; i < cmdLine.length(); ++i)
		{
			if (cmdLine[i] == '\"')
				inQuote = !inQuote;
			if (cmdLine[i] == ' ' && !inQuote)
			{
				if (inCmd)
				{
					inCmd = false;
					cmdEnd = i;
					// Do not store the first argument (executable name)
					if (!skipFirstArgument)
					{
						_arguments.push_back(cmdLine.substr(cmdStart, cmdEnd - cmdStart));
					}

					skipFirstArgument = false;
				}
			}
			else
			{
				if (!inCmd)
				{
					inCmd = true;
					cmdStart = i;
				}
			}
		}

		if (inCmd)
		{
			cmdEnd = cmdLine.length();
			if (!skipFirstArgument)
			{
				_arguments.push_back(cmdLine.substr(cmdStart, cmdEnd - cmdStart));
			}
		}

		// Strip double quotes from the arguments
		for (size_t i = 0; i < _arguments.size(); ++i)
		{
			_arguments[i] = StringUtils::Replace(_arguments[i], "\"", "");
		}

		return _arguments;
	}

	const std::vector<String>& ParseArguments(const char* cmdLine)
	{
		return ParseArguments(String(cmdLine));
	}

	const std::vector<String>& ParseArguments(const WString& cmdLine)
	{
		return ParseArguments(StringUtils::ToString(cmdLine));
	}

	const std::vector<String>& ParseArguments(const wchar_t* cmdLine)
	{
		return ParseArguments(StringUtils::ToString(cmdLine));
	}

	const std::vector<String>& ParseArguments(int argc, char** argv)
	{
		String cmdLine;

		for (int i = 0; i < argc; ++i)
		{
			cmdLine += StringUtils::Format("\"%s\" ", (const char*)argv[i]);
		}

		return ParseArguments(cmdLine);
	}

	const std::vector<String>& GetArguments()
	{
		return _arguments;
	}
}