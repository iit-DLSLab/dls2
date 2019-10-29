#ifndef DEBUG_HPP_5XQ3IR1T
#define DEBUG_HPP_5XQ3IR1T

#include <iostream>
#include <mutex>

#ifndef SOURCE_PATH_SIZE
#define SOURCE_PATH_SIZE 0
#endif

#define __FILENAME__ (__FILE__ + SOURCE_PATH_SIZE)

namespace debug
{
	namespace impl
	{
		extern std::mutex cout_mutex;
	}
}

#ifndef NDEBUG
#define DMSG(x) do{std::lock_guard<std::mutex> lock(debug::impl::cout_mutex); std::cout << __FILENAME__ << ": " << __PRETTY_FUNCTION__ << " " << x << std::endl;}while(0)
#define DLOG(x) do{std::lock_guard<std::mutex> lock(debug::impl::cout_mutex); std::cout << __FILENAME__ << ": " << __PRETTY_FUNCTION__ << " " << #x << ": " << x << std::endl;}while(0)
#else
#define DMSG(x) do{(void)sizeof(x);}while(0)
#endif

#endif /* end of include guard: DEBUG_HPP_5XQ3IR1T */
