/*
** Alimer - Copyright (C) 2016-2017 Amer Koleci.
**
** This file is subject to the terms and conditions defined in
** file 'LICENSE.md', which is part of this source code package.
*/

#include "Prerequisites.h"
#include "Logger.h"
#include "Core/Thread.h"
#include <ctime>
#include <cctype>
#include <cstdarg>
#include <fstream>

namespace Alimer
{
	static Logger* loggerInstance = 0;
	static std::ofstream _loggerStream;

	const char* logLevelPrefixes[] =
	{
		"DEBUG",
		"VERBOSE",
		"INFO",
		"WARNING",
		"ERROR",
		"FATAL",
		nullptr
	};

	Logger::Logger(bool timeStamp)
#ifdef _DEBUG
		: _level(LogLevel::Debug)
#else
		: _level(LogLevel::Verbose)
#endif
		, _timeStamp(timeStamp)
		, _quiet(false)
		, _inWrite(false)
		, _listeners(nullptr)
	{
		loggerInstance = this;
	}

	Logger::~Logger()
	{
		SafeDelete(_listeners);
		loggerInstance = nullptr;
	}

	void Logger::Open(const char* fileName)
	{
		_loggerStream.open(fileName);
	}

	void Logger::Close()
	{
		if (_loggerStream.is_open()) {
			_loggerStream.close();
		}
	}

	void Logger::SetLevel(LogLevel level)
	{
		if (level < LogLevel::Debug)
		{
			ALIMER_LOGERROR("Attempted to set erroneous log level %d", (uint32_t)level);
			return;
		}

		_level = level;
	}

	void Logger::SetTimeStamp(bool enable)
	{
		_timeStamp = enable;
	}

	void Logger::SetQuiet(bool quiet)
	{
		_quiet = quiet;
	}

	void Logger::Log(LogLevel level, const char* message, ...)
	{
		// No-op if illegal level
		if (level < LogLevel::Debug)
			return;

		// If not in the main thread, store message for later processing
		if (!Thread::IsMainThread())
		{
			if (loggerInstance)
			{
				//MutexLock lock(logInstance->logMutex_);
				//loggerInstance->threadMessages_.Push(StoredLogMessage(message, level, false));
			}

			return;
		}

		// Do not log if message level excluded or if currently sending a log event
		if (!loggerInstance || loggerInstance->_level > level || loggerInstance->_inWrite)
			return;

		// Declare a moderately sized buffer on the stack that should be
		// large enough to accommodate most log requests.
		int size = 1024;
		char stackBuffer[1024];
		std::vector<char> dynamicBuffer;
		char* str = stackBuffer;
		for (;;)
		{
			va_list args;
			va_start(args, message);

			// Pass one less than size to leave room for NULL terminator
			int needed = vsnprintf(str, size - 1, message, args);

			// NOTE: Some platforms return -1 when vsnprintf runs out of room, while others return
			// the number of characters actually needed to fill the buffer.
			if (needed >= 0 && needed < size)
			{
				// Successfully wrote buffer. Added a NULL terminator in case it wasn't written.
				str[needed] = '\0';
				va_end(args);
				break;
			}

			size = needed > 0 ? (needed + 1) : (size * 2);
			dynamicBuffer.resize(size);
			str = &dynamicBuffer[0];

			va_end(args);
		}

		String formattedMessage = logLevelPrefixes[(int)level];
		formattedMessage += ": " + String(str);

		if (loggerInstance->_timeStamp)
		{
			//formattedMessage = "[" + Time::GetTimeStamp() + "] " + formattedMessage;
		}

		if (_loggerStream.is_open())
		{
			_loggerStream << formattedMessage << std::endl;

			// Flush stcmdream to ensure it is written (incase of a crash, we need log to be up to date)
			_loggerStream.flush();
		}

		// Log to the default platform output
		Platform::Print("%s", formattedMessage.c_str());

		// Send log event.
		loggerInstance->_inWrite = true;

		if (loggerInstance->_listeners)
		{
			for (std::list<Logger::Listener*>::iterator itr = loggerInstance->_listeners->begin(); itr != loggerInstance->_listeners->end(); ++itr)
			{
				Logger::Listener* listener = *itr;
				listener->MessageLogged(level, str, formattedMessage.c_str());
			}
		}

		loggerInstance->_inWrite = false;
	}

	void Logger::AddListener(Logger::Listener* listener)
	{
		ALIMER_ASSERT(listener);

		if (_listeners == nullptr)
		{
			_listeners = new std::list<Logger::Listener*>();
		}

		_listeners->push_back(listener);
	}

	void Logger::RemoveListener(Logger::Listener* listener)
	{
		ALIMER_ASSERT(listener);

		if (_listeners)
		{
			for (std::list<Logger::Listener*>::iterator itr = _listeners->begin(); itr != _listeners->end(); ++itr)
			{
				if ((*itr) == listener)
				{
					_listeners->erase(itr);
					break;
				}
			}
		}
	}
}