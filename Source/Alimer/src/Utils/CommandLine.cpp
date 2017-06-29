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
	CommandLine::CommandLine()
	{

	}

	CommandLine::~CommandLine()
	{

	}

	void CommandLine::Parse(const WString& commandLine, bool skipFirstArgument)
	{
		return Parse(StringUtils::ToString(commandLine), skipFirstArgument);
	}

	void CommandLine::Parse(int argc, const char** argv, bool skipFirstArgument)
	{
		String commandLine;

		for (int i = 0; i < argc; ++i)
		{
			commandLine += StringUtils::Format("\"%s\" ", (const char*)argv[i]);
		}

		Parse(commandLine, skipFirstArgument);
	}

	void CommandLine::Parse(const String& commandLine, bool skipFirstArgument)
	{
		_args.clear();

		size_t cmdStart = 0, cmdEnd = 0;
		bool inCmd = false;
		bool inQuote = false;

		for (size_t i = 0; i < commandLine.length(); ++i)
		{
			if (commandLine[i] == '\"')
				inQuote = !inQuote;

			if (commandLine[i] == ' ' && !inQuote)
			{
				if (inCmd)
				{
					inCmd = false;
					cmdEnd = i;
					// Do not store the first argument (executable name)
					if (!skipFirstArgument)
					{
						_args.push_back(commandLine.substr(cmdStart, cmdEnd - cmdStart));
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
			cmdEnd = commandLine.length();
			if (!skipFirstArgument)
			{
				_args.push_back(commandLine.substr(cmdStart, cmdEnd - cmdStart));
			}
		}

		// Strip double quotes from the arguments
		for (size_t i = 0; i < _args.size(); ++i)
		{
			_args[i] = StringUtils::Replace(_args[i], "\"", "");
		}
	}

	String CommandLine::FindOption(const String& longName, const String& defaultValue) const
	{
		const char* result = Find('\0', longName, 1);
		return result == nullptr ? defaultValue : result;
	}

	String CommandLine::FindOption(const char shortName, const String& longName, const String& defaultValue) const
	{
		const char* result = Find(shortName, longName, 1);
		return result == nullptr ? defaultValue : result;
	}

	const char* CommandLine::FindOption(const String& longName, size_t numParams) const
	{
		const char* result = Find('\0', longName, numParams);
		return result;
	}

	const char* CommandLine::FindOption(const char shortName, const String& longName, size_t numParams) const
	{
		const char* result = Find(shortName, longName, numParams);
		return result;
	}

	bool CommandLine::HasArg(const char shortName, const String& longName) const
	{
		const char* arg = FindOption(shortName, longName, 0);
		return arg != nullptr;
	}

	bool CommandLine::HasArg(const String& longName) const
	{
		const char* arg = FindOption('\0', longName, 0);
		return arg != nullptr;
	}

	String CommandLine::GetString(const String& arg, const String& defaultValue) const
	{
		auto it = std::find(_args.begin(), _args.end(), arg);
		if (it != _args.end()) {
			return *it;
		}

		return defaultValue;
	}

	int CommandLine::GetInt(const String& arg, int defaultValue) const
	{
		auto it = std::find(_args.begin(), _args.end(), arg);
		if (it != _args.end()) {
			return StringUtils::ParseInt(*it);
		}

		return defaultValue;
	}

	uint32_t CommandLine::GetUInt(const String& arg, uint32_t defaultValue) const
	{
		auto it = std::find(_args.begin(), _args.end(), arg);
		if (it != _args.end()) {
			return StringUtils::ParseUInt(*it);
		}

		return defaultValue;
	}

	float CommandLine::GetFloat(const String& arg, float defaultValue) const
	{
		auto it = std::find(_args.begin(), _args.end(), arg);
		if (it != _args.end()) {
			return StringUtils::ParseFloat(*it);
		}

		return defaultValue;
	}

	const char* CommandLine::Find(const char shortName, const String& longName, size_t numParams) const
	{
		for (size_t i = 0; i < _args.size(); ++i)
		{
			const char* arg = _args[i].c_str();
			if ('-' == *arg)
			{
				++arg;
				if (shortName == *arg)
				{
					if (1 == strlen(arg))
					{
						if (numParams == 0)
						{
							return "";
						}
						else if (
							i + numParams < _args.size() &&
							'-' != *_args[i + 1].c_str())
						{
							return _args[i + 1].c_str();
						}

						return nullptr;
					}
				}
				else if (
					!longName.empty() &&
					*arg == '-' &&
					StringUtils::Compare(arg + 1, longName.c_str(), false) == 0)
				{
					if (0 == numParams)
					{
						return "";
					}
					else if (i + numParams < _args.size()
						&& '-' != *_args[i + 1].c_str())
					{
						return _args[i + 1].c_str();
					}

					return  nullptr;
				}
			}
		}

		return nullptr;
	}

	const std::vector<String>& CommandLine::GetArgs() const
	{
		return _args;
	}
}