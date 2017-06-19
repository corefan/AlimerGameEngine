/*
** Alimer - Copyright (C) Amer Koleci
**
** This file is subject to the terms and conditions defined in
** file 'LICENSE.txt', which is part of this source code package.
*/

#pragma once

#include <list>

namespace Alimer
{
	/**
	* Enumeration of valid log levels.
	*/
	enum class LogLevel
	{
		Debug,
		Verbose,
		Info,
		Warning,
		Error,
		Fatal
	};


	/**
	* Provides a logging system for the engine.
	*/
	class ALIMER_API Logger
	{
	public:
		/**
		* Constructor.
		*/
		Logger(bool timeStamp = true);

		/**
		* Destructor.
		*/
		virtual ~Logger();

		/// Open the log file.
		void Open(const char* fileName);

		/// Close the log file.
		void Close();

		/// Set logging level.
		void SetLevel(LogLevel level);
		/// Set whether to timestamp log messages.
		void SetTimeStamp(bool enable);
		/// Set quiet mode ie. only print error entries to standard error stream (which is normally redirected to console also). Output to log file is not affected by this mode.
		void SetQuiet(bool quiet);

		/// Return logging level.
		LogLevel GetLevel() const { return _level; }

		/// Return whether log messages are timestamped.
		bool GetTimeStamp() const { return _timeStamp; }

		/// Return whether log is in quiet mode (only errors printed to standard error stream).
		bool IsQuiet() const { return _quiet; }

		/**
		* Logs a message at the specified log level.
		*
		* @param level Log level.
		* @param message Log message.
		*/
		static void Log(LogLevel level, const char* message, ...);

		/**
		* Listener interface for Log events.
		*/
		class ALIMER_API Listener
		{
		public:
			virtual ~Listener() { }

			/**
			* This is called whenever the log receives a message and is about to write it out
			*
			* @param node The Node object that was changed.
			*/
			virtual void MessageLogged(LogLevel level, const char* message, const char* formattedMessage) = 0;
		};

		/**
		* Adds a log listener.
		*
		* @param listener The listener to add.
		*/
		void AddListener(Logger::Listener* listener);

		/**
		* Removes a log listener.
		*
		* @param listener The listener to remove.
		*/
		void RemoveListener(Logger::Listener* listener);

	private:
		Logger(const Logger&) = delete;
		Logger& operator=(const Logger&) = delete;

		LogLevel _level;
		bool _timeStamp;

		/// Quiet mode flag.
		bool _quiet;
		bool _inWrite;

		/**
		* List of Listener's on the Log.
		*/
		std::list<Listener*>* _listeners;
	};
}

#ifndef ALIMER_DISABLE_LOGGING
#	if defined(_MSC_VER) && defined(_DEBUG)
#		define DEBUG_BREAK() __debugbreak()
#	else
#	define DEBUG_BREAK()
#endif

// Current function macro.
#if defined(_MSC_VER)
#	define __current__func__ __FUNCTION__
#else
#	define __current__func__ __func__
#endif

// Error macro.
#ifdef ALIMER_ERRORS_AS_WARNINGS
#define ALIMER_ERROR ALIMER_WARN
#else
#define ALIMER_ERROR(...) do \
{ \
	Alimer::Logger::Log(Alimer::LogLevel::Error, "%s -- ", __current__func__); \
	Alimer::Logger::Log(Alimer::LogLevel::Error, __VA_ARGS__); \
	Alimer::Logger::Log(Alimer::LogLevel::Error, "\n"); \
	DEBUG_BREAK(); \
	assert(0); \
	std::exit(-1); \
} while (0)
#endif

// Warning macro.
#define ALIMER_WARN(...) do \
{ \
	Alimer::Logger::Log(Alimer::LogLevel::Warning, "%s -- ", __current__func__); \
	Alimer::Logger::Log(Alimer::LogLevel::Warning, __VA_ARGS__); \
	Alimer::Logger::Log(Alimer::LogLevel::Warning, "\n"); \
} while (0)

#define ALIMER_LOG_MESSAGE(...) do \
{ \
	Alimer::Logger::Log(Alimer::LogLevel::Info, __VA_ARGS__); \
	Alimer::Logger::Log(Alimer::LogLevel::Info, "\n"); \
} while (0)

#define ALIMER_LOGDEBUG(...) Alimer::Logger::Log(Alimer::LogLevel::Debug, __VA_ARGS__)
#define ALIMER_LOGINFO(...) Alimer::Logger::Log(Alimer::LogLevel::Info, __VA_ARGS__)
#define ALIMER_LOGWARNING(...) Alimer::Logger::Log(Alimer::LogLevel::Warning, __VA_ARGS__)
#define ALIMER_LOGERROR(...) Alimer::Logger::Log(Alimer::LogLevel::Error, __VA_ARGS__)

#else

#define ALIMER_ERROR(...)
#define ALIMER_WARN(...)
#define ALIMER_LOG_MESSAGE(...) 

#define ALIMER_LOGDEBUG(...)
#define ALIMER_LOGINFO(...)
#define ALIMER_LOGWARNING(...)
#define ALIMER_LOGERROR(...)

#endif