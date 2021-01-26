#ifndef LOG_TRACE_HPP_74d9b032_2886_44a0_a696_5c23114ce664
#define LOG_TRACE_HPP_74d9b032_2886_44a0_a696_5c23114ce664

#include <ostream>
#include <sstream>

#ifndef NDEBUG
	#define DLS_TRACE(...)                         \
	{                                              \
		std::stringstream ss;                      \
		dls::log_trace::trace(ss, __VA_ARGS__);    \
		ss << " " << __PRETTY_FUNCTION__;          \
		std::cout << ss.str() << std::endl;        \
	}
#else
	#define DLS_TRACE(...)
#endif

namespace dls
{
	namespace log_trace
	{
		template <typename T>
		void trace(std::ostream &os, T t)
		{
			os << t;
		}

		template <typename T, typename... Us>
		void trace(std::ostream &os, T t, Us...us)
		{
			os << t;
			trace(os, us...);
		}
	}
}

#endif // LOG_TRACE_HPP_74d9b032_2886_44a0_a696_5c23114ce664
