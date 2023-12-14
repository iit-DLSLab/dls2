# Periodic plugin

## Introduction
This project skeleton represents the starting point of anyone that wants to create a C++ plugin to interface a software module with DLS2.

This project has been created with the command

        create_periodic_plugin

that requests you to provide a *Plugin Type* and a *Plugin Name* (which is also the name of the module).

The project structure is made by the following main folders:
* module: where you develop your module
* plugin/core: where you develop the plugin
* plugin/console_commands: where you develop your console commands
* plugin/messages: where you develop custom messages
* plugin/topics: where you develop custom topics

The Plugin, module and console commands classes are automatically created, together with some suggestions on how to customize your plugin.

Notice that the include directories has a subdirectory following this convention

        include/dls/<plugin_type>/<plugin_name>

where *\<plugin_name>* is the name of the plugin, and *\<plugin_type>* is the plugin type that can be: *hardwares, estimators, controllers, motion_generators*. This include structure helps with the inclusion of the headers in other libraries.

Don't worry, you are lucky: we will guide you step-by-step with the customization of your plugin.

## Hands-on
In this section we will cover each .hpp and .cpp file, describing how to replace the placeholders with an example. The example considers the creation of a plugin for a stance detection module. For each placeholder to substitute, it is provided the skeleton and then an example.

### Create the software module

### Create the plugin
Let's start with the outermost CMakeLists.txt.

* set the project name (**#[[/*plugin_library_name*/]]**)

        project(#[[/*plugin_library_name*/]]_plugin)

  e.g.

        project(stance_detection)

* set the module library name (**#[[/*module_library_name*/]]**)

        set(MODULE_LIBRARY_NAME #[[/*"module_library_name"*/]])
  e.g.

        set(MODULE_LIBRARY_NAME "stance_detector")

Let's continue with plugin/core/CMakeLists.txt.

* set the plugin type (**#[[/*plugin_type*/]]**)

        install(
            DIRECTORY   include/
            COMPONENT   stance_detection_dev
            DESTINATION /usr/include/dls2/#[[/*plugin_type*/]]/${PROJECT_NAME}
        )
        install(
            TARGETS             ${PROJECT_NAME}
            LIBRARY DESTINATION /usr/lib/dls2/#[[/*plugin_type*/]]
            COMPONENT           ${PROJECT_NAME}
        )
  e.g.
        
        install(
	        DIRECTORY   include/
	        COMPONENT   stance_detection_dev
	        DESTINATION /usr/include/dls2/estimators/stance_detection_plugin
        )

        install(
	        TARGETS {PROJECT_NAME}
	        LIBRARY DESTINATION /usr/lib/dls2/estimators
	        COMPONENT           ${PROJECT_NAME}
        )


  The *plugin_type* can be: *hardwares, estimators, controllers, motion_generators*

Consider now plugin/core/include/plugin.hpp
* set the header guard, writing the plugin class name in uppercase letter (**/\*PLUGIN_CLASS_NAME\*/**)

        #ifndef /*PLUGIN_CLASS_NAME*/_HPP
        #define /*PLUGIN_CLASS_NAME*/_HPP

  e.g.

        #ifndef STANCE_DETECTION_PLUGIN_HPP
        #define STANCE_DETECTION_PLUGIN_HPP
* include the module header (**/\*path_to_module_header\*/**)

        #include "/*path_to_module_header*/"
  e.g.

        #include "dls/estimators/stance_detection/stance_detector.hpp"

* include the console command header (**/\*path_to_console_commands_header\*/**)
        
        #include "/*path_to_console_commands_header*/"

  e.g.
        
        #include "dls/estimators/stance_detection/console_commands.hpp"

* include the headers of the message wrappers, for defining inputs and outputs variables (**/\*paths_to_wrapper_header\*/**)

        */paths_to_wrapper_header*/
  e.g.

        #include "dls2/msg_wrappers/blind_state.hpp" // input, off-the-shelf
        #include "dls2/msg_wrappers/base_state.hpp" // input, off-the-shelf
        #include "dls2/msg_wrappers/base_state.hpp" // input, off-the-shelf
        #include "dls/estimators/stance_detection/stance_status.hpp"// output, custom
* set the namespace

        namespace /*plugin_type*/
  e.g.

        namespace estimators

        /*plugin_type*/
  type of the plugin, as before

        /*plugin_class_name*/
  name of the class of the plugin

        /*aguments_of_module_constructor*/
  add here the constructor arguments needed to construct your module

        /*module_class_name*/
  name of the class defined in the module

        /*module_variable_name_*/
  name of the variable identyfing the module object
  
        /*define_inputs*/
        /*define_output*/
  define the inputs and outputs variables of the plugin; their types are message wrapper classes

* plugin/core/src/plugin.cpp

        /*path_to_plugin_header*/
  path to the plugin.hpp file, relative to include folder

        /*include_paths_to_topic_header*/
  includes the paths to the headers of the topic

        /*plugin_type*/
        /*plugin_class_name*/
        /*aguments_of_module_constructor*/
        /*module_variable_name*/
  as before

        /*construct_input_variables*/
  construct the input variables

        /*construct_output_variables*/
  construct the output variables

        /*message_wrapper_class*/
  class name of the message wrapper associated to an input or output

        /*type_of_arguments_of_message_constructor*/
  class name of the arguments of the message constructor

        /*topic_name*/
  topic used for an input or output

        /*input_variable_name*/
  name of the variable used as input or output

        /*arguments_of_message_constructor*/
  arugments for the message constructor

### Create custom console commands
Substitute the placeholders in

* plugin/core/plugin.hpp

        /*path_to_console_commands_header*/
  path to the console_commands.hpp file

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
