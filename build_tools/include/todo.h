#ifndef TODO_H_VKP6FJO9
#define TODO_H_VKP6FJO9

#ifndef SOURCE_PATH_SIZE
#define SOURCE_PATH_SIZE 0
#endif

#include <string.h>

#define __FILENAME__ (__FILE__ + SOURCE_PATH_SIZE)

#define S1(x) #x
#define S2(x) S1(x)
#define LOCATION __FILE__ "." S2(__LINE__)
#define LOCATION_MESSAGE(msg) LOCATION " " msg

#ifdef SHOW_TODOS_AT_COMPILE_TIME
#define TODO(x) _Pragma("message \"TODO\"")
#else
#define TODO(x)
#endif

#endif /* end of include guard: TODO_H_VKP6FJO9 */
