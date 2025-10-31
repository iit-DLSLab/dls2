# Description of the custom plugin example
- include/custom_plugin.hpp
- src/custom_plugin.cpp
- src/main.cpp

# compile and install test by compiling dls2
# launch test from build directory as an executable
```
sudo ./../dls2/bin/Release/custom_plugin_basic_test
```
sudo is used for setting RT priority to the process. Then activate the plugin from the dls2 console.
# launch test from dls2 console
```
loadPlugin custom_plugin
```
Then activate the plugin from the dls2 console.