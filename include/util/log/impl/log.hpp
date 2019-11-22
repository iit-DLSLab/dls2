/*******************************************************************************
*                                                       ,----,                 *
*                                                     .'   .' \                *
*                                                   ,----,'    |               *
*               ________  ___       ________        |    :  .  ;               *
*              |\   ___ \|\  \     |\   ____\       ;    |.'  /                *
*              \ \  \_|\ \ \  \    \ \  \___|_      `----'/  ;                 *
*               \ \  \ \\ \ \  \    \ \_____  \       /  ;  /                  *
*                \ \  \_\\ \ \  \____\|____|\  \     ;  /  /-,                 *
*                 \ \_______\ \_______\____\_\  \   /  /  /.`|                 *
*                  \|_______|\|_______|\_________\./__;      :                 *
*                                     \|_________||   :    .'                  *
*                                                 ;   | .'                     *
*                                                 `---'                        *
********************************************************************************
* Author:            Hendrik de Bruin                                          *
* Maintainer:        Hendrik de Bruin                                          *
* author email:      hendrik.debruin@iit.it                                    *
*******************************************************************************/
#ifndef LOG_HPP_8TZQXSVU
#define LOG_HPP_8TZQXSVU

#include "util/log/log.hpp"
#include "util/messaging/publisher_base.hpp"
#include "msg/stringmsgPubSubTypes.h"


namespace dls
{
namespace logging
	{
		/// Implementation namespace
		///
		/// Do not use this namespace in user code. This namespace contains
		/// definitions for helper classes for the logging library
		namespace impl
		{
			/// Log levels
			///
			enum class LogLevel
			{
				DEBUG,	///< Debug information stream
				INFO,	///< Normal user communication stream
				WARN,	///< Warning stream
				ERROR,	///< Non-fatal error messages stream
				FATAL	///< Fatal error messages stream
			};

			// forward declaration
			template <LogLevel loglevel>
			class LogStream;

			/// Log stream manipulation
			///
			/// Defines manipulations that can be performed on a log stream
			enum class StreamManipulator
			{
				ENDL	///< Indicate end of log
			};

			/// Log Manipulation class
			///
			/// Empty class that wraps StreamManipulator. Can be passed into a
			/// log stream to perform manipulations
			template <StreamManipulator manip>
			class StreamManip{};

			/// Log Helper class
			///
			/// This class is instantiated when a write is made to a LogStream.
			/// It keeps track of all the writes that are made to the log stream
			/// and sends it in one go to the main log stream class when it
			/// encounters logging::StreamManip<ENDL>. That way, an entire log
			/// stream is written such that concurrent writes won't interleave.
			template <LogLevel loglevel>
			class LogInput
			{
				friend class LogStream<loglevel>;
			public:
				/// Start logging
				///
				/// Adds the string argument to the log written ;o far
				/// @return a reference to this object
				LogInput<loglevel> &operator<< (const std::string&);

				/// End Logging
				///
				/// Sends the log off to the LogStream class to write out to its
				/// destination in one go
				void operator<<(const StreamManip<StreamManipulator::ENDL>&);

			private:
				/// Private constructor
				///
				/// This class can only be instantiated using LogStream
				LogInput();

				/// The log written so far
				///
				std::stringstream ss;
			};

			/// Logging class
			///
			/// This class takes care of logging to various locations in a
			/// thread-safe manner
			template <LogLevel loglevel>
			class LogStream
			{
				friend class LogInput<loglevel>;
			public:
				/// Start logging
				///
				/// @return a LogInput object which takes care of building the
				/// log and sending it back as a single string object. This is
				/// done to prevent interleaved writes from concurrent threads
				LogInput<loglevel> operator<< (const std::string&) const;
			private:

				/// Write a log
				///
				/// This is used by LogInput
				static void log(const std::string&);

				/// Publisher that writes out the log
				///
				static const std::string topic;

				/// Topic where this stream is published
				///
				// pointer because fastrtps doesn't like a static member for
				// some reason
				static std::shared_ptr<PublisherBase<StringMsgPubSubType>> pPublisher;
			};
		} // namespace impl
	} // namespace logging
} // namespace dls

#include "util/log/log.tpp"

#endif /* end of include guard: LOG_HPP_8TZQXSVU */

