/*
** Alimer - Copyright (C) 2016-2017 Amer Koleci.
**
** This file is subject to the terms and conditions defined in
** file 'LICENSE.md', which is part of this source code package.
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
		static void Log(LogLevel level, const String& message);

		/**
		* Logs a message at the specified log level.
		*
		* @param level Log level.
		* @param format The format string.
		*/
		static void LogFormat(LogLevel level, const char* format, ...);

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

#define ALIMER_LOGDEBUG(message)	Alimer::Logger::Log(Alimer::LogLevel::Debug, message)
#define ALIMER_LOGINFO(message)		Alimer::Logger::Log(Alimer::LogLevel::Info, message)
#define ALIMER_LOGWARNING(message)	Alimer::Logger::Log(Alimer::LogLevel::Warning, message)
#define ALIMER_LOGERROR(message)	Alimer::Logger::Log(Alimer::LogLevel::Error, message)
#define ALIMER_LOGFATAL(message)	Alimer::Logger::Log(Alimer::LogLevel::Fatal, message)

#define ALIMER_LOGDEBUGF(format, ...)	Alimer::Logger::LogFormat(Alimer::LogLevel::Debug, format, ##__VA_ARGS__)
#define ALIMER_LOGINFOF(format, ...)	Alimer::Logger::LogFormat(Alimer::LogLevel::Info, format, ##__VA_ARGS__)
#define ALIMER_LOGWARNINGF(format, ...) Alimer::Logger::LogFormat(Alimer::LogLevel::Warning, format, ##__VA_ARGS__)
#define ALIMER_LOGERRORF(format, ...)	Alimer::Logger::LogFormat(Alimer::LogLevel::Error, format, ##__VA_ARGS__)
#define ALIMER_LOGFATALF(format, ...)	Alimer::Logger::LogFormat(Alimer::LogLevel::Fatal, format, ##__VA_ARGS__)

#else

#define ALIMER_LOGDEBUG(message) ((void)0)
#define ALIMER_LOGINFO(message) ((void)0)
#define ALIMER_LOGWARNING(message) ((void)0)
#define ALIMER_LOGERROR(message) ((void)0)
#define ALIMER_LOGFATAL(message) ((void)0)

#define ALIMER_LOGDEBUGF(format, ...) ((void)0)
#define ALIMER_LOGINFOF(format, ...) ((void)0)
#define ALIMER_LOGWARNINGF(format, ...) ((void)0)
#define ALIMER_LOGERRORF(format, ...) ((void)0)
#define ALIMER_LOGFATALF(format, ...) ((void)0)

#endif