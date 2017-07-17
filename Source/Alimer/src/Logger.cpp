/*
** Alimer - Copyright (C) 2016-2017 Amer Koleci.
**
** This file is subject to the terms and conditions defined in
** file 'LICENSE.md', which is part of this source code package.
*/

#include "Prerequisites.h"
#include "Logger.h"
#include "Core/Thread.h"
#include "Utils/StringUtils.h"
#include <ctime>
#include <cctype>
#include <cstdarg>
#include <fstream>

#if ALIMER_MICROSOFT_FAMILY
#	include <windows.h>
#	include <strsafe.h>
#endif

#if ALIMER_ANDROID
#include <android/log.h>
#endif

#if ALIMER_EMSCRIPTEN
#	include <emscripten.h>
#endif

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
			ALIMER_LOGERRORF("Attempted to set erroneous log level %d", (uint32_t)level);
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

	void Logger::Log(LogLevel level, const String& message)
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

		String formattedMessage = logLevelPrefixes[(int)level];
		formattedMessage += ": " + message;

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

#if ALIMER_UNIX_FAMILY
		switch (level)
		{
		case LogLevel::Fatal:
		case LogLevel::Error:
		case LogLevel::Warning:
			std::cerr << formattedMessage << std::endl;
			break;

		default:
			std::cout << formattedMessage << std::endl;
			break;
		}

#elif ALIMER_MICROSOFT_FAMILY
		std::vector<wchar_t> szBuffer(formattedMessage.length() + 2);
		MultiByteToWideChar(CP_UTF8, 0, formattedMessage.c_str(), -1, szBuffer.data(), static_cast<int>(szBuffer.size()));
		StringCchCatW(szBuffer.data(), szBuffer.size(), L"\n");
		OutputDebugStringW(szBuffer.data());

#ifdef _DEBUG
		HANDLE handle = 0;
		switch (level)
		{
		case LogLevel::Error:
		case LogLevel::Warning:
		case LogLevel::Fatal:
			handle = GetStdHandle(STD_ERROR_HANDLE);
			break;

		default:
			handle = GetStdHandle(STD_OUTPUT_HANDLE);
			break;
		}

		if (handle)
		{
			DWORD bytesWritten;
			WriteConsoleW(handle, szBuffer.data(), static_cast<DWORD>(wcslen(szBuffer.data())), &bytesWritten, nullptr);
		}
#endif

#elif ALIMER_ANDROID
		int priority = ANDROID_LOG_INFO;
		switch (level)
		{
		case LogLevel::Error:
			priority = ANDROID_LOG_ERROR;
			break;

		case LogLevel::Fatal:
			priority = ANDROID_LOG_FATAL;
			break;

		case LogLevel::Warning:
			priority = ANDROID_LOG_WARN;
			break;

		case LogLevel::Debug:
			priority = ANDROID_LOG_DEBUG;
			break;

		case LogLevel::Verbose:
			priority = ANDROID_LOG_VERBOSE;
			break;

		default:  break;
		}
		__android_log_print(priority, "Alimer", "%s", message.c_str());

#elif ALIMER_EMSCRIPTEN
		int flags = EM_LOG_CONSOLE;
		if (level == LogLevel::Error)
		{
			flags |= EM_LOG_ERROR;
		}
		else if (level == LogLevel::Warning)
		{
			flags |= EM_LOG_WARN;
		}
		else if (level == LogLevel::Fatal)
		{
			flags |= EM_LOG_ERROR | EM_LOG_C_STACK | EM_LOG_JS_STACK | EM_LOG_DEMANGLE;
		}
		emscripten_log(flags, "%s", str);

		return;
#endif
		// Send log event.
		loggerInstance->_inWrite = true;

		if (loggerInstance->_listeners)
		{
			for (std::list<Logger::Listener*>::iterator itr = loggerInstance->_listeners->begin(); itr != loggerInstance->_listeners->end(); ++itr)
			{
				Logger::Listener* listener = *itr;
				listener->MessageLogged(level, message.c_str(), formattedMessage.c_str());
			}
		}

		loggerInstance->_inWrite = false;
	}

	void Logger::LogFormat(LogLevel level, const char* format, ...)
	{
		va_list args;
		va_start(args, format);
		String message = StringUtils::Format(format, args);
		Log(level, message);
		va_end(args);
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