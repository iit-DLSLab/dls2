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
#ifndef LOG_HPP_AMCC9JXS
#define LOG_HPP_AMCC9JXS

#include <sstream>
#include <mutex>

#include "util/log/impl/log.hpp"

namespace dls
{
	/// Logging namespace
	///
	/// This namespace contains util classes related to logging, as well as
	/// instances of those classes for use
	namespace logging
	{
		/// Debug messages log stream
		///
		extern impl::LogStream<impl::LogLevel::DEBUG>  cdbg;

		/// Stream for basic system logging, such as state changes
		///
		extern impl::LogStream<impl::LogLevel::INFO>   clog;

		/// Stream for warnings that the user should be made aware of
		///
		extern impl::LogStream<impl::LogLevel::WARN>   cout;

		/// Stream for non-fatal errors
		///
		extern impl::LogStream<impl::LogLevel::ERROR>  cerr;

		/// Stream for fatal error messages
		///
		extern impl::LogStream<impl::LogLevel::FATAL>  cfatal;

		/// Stream manipulator indicating the end of a log stream
		///
		extern impl::StreamManip<impl::StreamManipulator::ENDL> endl;

	} // namespace logging
} // namespace dls

#endif /* end of include guard: LOG_HPP_AMCC9JXS */
