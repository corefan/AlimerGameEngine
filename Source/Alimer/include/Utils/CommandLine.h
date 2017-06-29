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
	class ALIMER_API CommandLine
	{
	public:
		/**
		* Constructor.
		*/
		CommandLine();

		/**
		* Destructor.
		*/
		~CommandLine();

		void Parse(const WString& commandLine, bool skipFirstArgument = true);
		void Parse(int argc, const char** argv, bool skipFirstArgument = true);
		void Parse(const String& commandLine, bool skipFirstArgument = true);
		
		String FindOption(const String& longName, const String& defaultValue) const;
		String FindOption(const char shortName, const String& longName, const String& defaultValue) const;
		const char* FindOption(const String& longName, size_t numParams = 1) const;
		const char* FindOption(const char shortName, const String& longName = "", size_t numParams = 1) const;

		bool HasArg(const char shortName, const String& longName = "") const;
		bool HasArg(const String& longName = "") const;

		String GetString(const String& arg, const String& defaultValue = "") const;
		int GetInt(const String& arg, int defaultValue = 0) const;
		uint32_t GetUInt(const String& arg, uint32_t defaultValue = 0) const;
		float GetFloat(const String& arg, float defaultValue = 0.0f) const;

		/**
		* Gets vector of arguments.
		*/
		const std::vector<String>& GetArgs() const;

	private:
		const char* Find(const char shortName, const String& longName, size_t numParams) const;
		
		std::vector<String> _args;
	};
}