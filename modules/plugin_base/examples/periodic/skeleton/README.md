### Create the plugin
* substitute the placeholders in plugin/core/plugin.hpp, plugin/core/plugin.cpp and plugin/core/CMakeLists.txt.
### Create custom console commands
* substitute the placeholders in console_commands.hpp, console_commands.cpp and plugin/console_commands/CMakeLists.txt.

* in the constructor fo the console command class, each added console command needs to be registered to the command manager, as shown in [examples/periodic/dummy_plugin](../dummy_plugin/).

* in periodic/CMakelists.txt, decomment `add_subdirectory(console_commands)`
### Create custom messages
TODO

* in periodic/CMakelists.txt, decomment `add_subdirectory(messages)`
### Create custom topics
* substitute the placeholders in topics.hpp, topics.cpp and plugin/topics/CMakeLists.txt.

* in periodic/CMakelists.txt, decomment `add_subdirectory(topics)`

### How to set the scheduler properties
In the plugin/config folder you have the possibility to set the properties of the scheduler.
