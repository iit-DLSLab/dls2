// #ifndef LOG_TPP_O9SQ6AB1
// #define LOG_TPP_O9SQ6AB1

// #include "util/log/log.hpp"

// template <logging::impl::LogLevel loglevel>
// logging::impl::LogInput<loglevel> operator<<
// (
// 	const logging::impl::LogStream<loglevel> &ls,
// 	const std::string &s
// )
// {
// 	logging::impl::LogInput<loglevel> input;
// 	input.ss << s;
// 	return input;
// }

// template <logging::impl::LogLevel loglevel>
// logging::impl::LogInput<loglevel> &operator<<
// (
// 	const logging::impl::LogInput<loglevel> &li,
// 	const std::string &s
// )
// {
// 	li.ss << s;
// 	return li;
// }

// template <logging::impl::LogLevel loglevel>
// void operator<<
// (
// 	const logging::impl::LogInput<loglevel> &li,
// 	const logging::impl::StreamManip<logging::impl::StreamManipulator::ENDL>&
// )
// {
// 	logging::impl::LogStream<loglevel>::log(li.ss.str());
// }
// #endif /* end of include guard: LOG_TPP_O9SQ6AB1 */

