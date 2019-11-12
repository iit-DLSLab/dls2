// path prefix for child process executables
#ifndef NDEBUG
	// if debug, look in current directory
	#define CHILD_PROCES_PATH
#else
	// if release, look in install directory
	#define CHILD_PROCESS_PATH "/usr/bin/"
#endif

// where shared objects are located
#ifndef NDEBUG
	// if debug, look in current directory
	#define LIBRARY_PROCESS_PATH "./"
#else
	// if release, look in standard locations
	#define LIBRARY_PROCESS_PATH
#endif
