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
#ifndef LOG_TPP_O9SQ6AB1
#define LOG_TPP_O9SQ6AB1

#include "util/log/log.hpp"

#include <iostream>
#include <iomanip>
#include <ctime>

#include "topics/debug_log_stream.hpp"
#include "topics/error_log_stream.hpp"
#include "topics/fatal_log_stream.hpp"
#include "topics/info_log_stream.hpp"
#include "topics/warn_log_stream.hpp"

namespace dls
{
	namespace logging
	{
		namespace impl
		{
			template <LogLevel loglevel>
			LogInput<loglevel>::LogInput():
				ss()
			{ }

			template <LogLevel loglevel>
			LogInput<loglevel> &LogInput<loglevel>::operator<<(const std::string &s)
			{
				this->ss << s;
				return *this;
			}

			template <LogLevel loglevel>
			void LogInput<loglevel>::operator<<
			(
				const StreamManip<StreamManipulator::ENDL>&
			)
			{
				LogStream<loglevel>::log(this->ss.str());
			}

			template <LogLevel loglevel>
			LogInput<loglevel> LogStream<loglevel>::operator<<(const std::string &s) const
			{
				logging::impl::LogInput<loglevel> input;

				// log the time
				auto t = std::time(nullptr);
				auto tm = *std::localtime(&t);
				input.ss << std::put_time(&tm, "%d/%m/%Y %H:%M:%S");
				input.ss << ": ";

				// record the message
				input.ss << s;

				return input;
			}

			template <LogLevel loglevel>
			void LogStream<loglevel>::log(const std::string &s)
			{
				if(!LogStream<loglevel>::pPublisher)
				{
					LogStream<loglevel>::pPublisher =
						std::make_shared<PublisherBase<StringMsgPubSubType>>
						(
							LogStream<loglevel>::topic
						);
				}

				StringMsg msg;
				msg.msg(s);
				LogStream<loglevel>::pPublisher->publish(msg);
			}
		} // namespace impl
	} // namespace logging
} // namespace dls
#endif /* end of include guard: LOG_TPP_O9SQ6AB1 */
