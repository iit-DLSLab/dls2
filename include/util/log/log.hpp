#ifndef LOG_HPP_AMCC9JXS
#define LOG_HPP_AMCC9JXS

#include <sstream>
#include <mutex>

#include "util/log/impl/log.hpp"

/// Logging namespace
///
/// This namespace contains util classes related to logging, as well as
/// instances of those classes for use
namespace logging
{
	/// Debug messages log stream
	///
	impl::LogStream<impl::LogLevel::DEBUG>  cdbg;

	/// Stream for basic system logging, such as state changes
	///
	impl::LogStream<impl::LogLevel::INFO>   clog;

	/// Stream for warnings that the user should be made aware of
	///
	impl::LogStream<impl::LogLevel::WARN>   cout;

	/// Stream for non-fatal errors
	///
	impl::LogStream<impl::LogLevel::ERROR>  cerr;

	/// Stream for fatal error messages
	///
	impl::LogStream<impl::LogLevel::FATAL>  cfatal;

	/// Stream manipulator indicating the end of a log stream
	///
	impl::StreamManip<impl::StreamManipulator::ENDL> endl;

} // namespace logging


#endif /* end of include guard: LOG_HPP_AMCC9JXS */
