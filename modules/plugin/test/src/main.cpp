#include "hello_world_plugin.hpp"

int main()
{
    HelloWorldPlugin plugin("hello_world_plugin"); // same name of the library the HelloWorldPlugin is compiled into
    plugin.execute();
	return 0;
}