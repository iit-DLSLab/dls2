#include "safety_plugin.hpp"

int main()
{
    SafetyPlugin plugin("safety_plugin"); // same name of the library the CustomPlugin is compiled into
    plugin.execute();
	return 0;
}