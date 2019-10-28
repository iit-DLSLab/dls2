#ifndef DEBUG_HPP_5XQ3IR1T
#define DEBUG_HPP_5XQ3IR1T

#include <iostream>

#ifndef SOURCE_PATH_SIZE
#define SOURCE_PATH_SIZE 0
#endif

#define __FILENAME__ (__FILE__ + SOURCE_PATH_SIZE)

#ifndef NDEBUG
#define DMSG(x) do{std::cout << __FILENAME__ << ": " << __PRETTY_FUNCTION__ << " " << x << std::endl;}while(0)
#else
#define DMSG(x) do{(void)sizeof(x);}while(0)
#endif

#endif /* end of include guard: DEBUG_HPP_5XQ3IR1T */
