#ifndef TODO_H_VKP6FJO9
#define TODO_H_VKP6FJO9

// #define S1(x) #x
// #define S2(x) S1(x)
// #define LOCATION __FILE__ "." S2(__LINE__)
// #define LOCATION_MESSAGE(msg) LOCATION " " msg

#ifdef SHOW_TODOS_AT_COMPILE_TIME
#define TODO(x) _Pragma("message \"TODO\"")
#else
#define TODO(x)
#endif

#endif /* end of include guard: TODO_H_VKP6FJO9 */
