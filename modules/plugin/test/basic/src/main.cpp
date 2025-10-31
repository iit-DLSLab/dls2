#include "custom_plugin.hpp"

int main()
{
    CustomPlugin plugin("custom_plugin_basic_test"); // same name of the library/executable the CustomPlugin is compiled into
    plugin.execute();
	return 0;
}