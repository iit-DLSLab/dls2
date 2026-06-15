# Plugin

## Introduction
This C++ library provides an interface to connect an external software module to DLS2.

#### Prerequisites
Read at least at the [Introduction](https://fast-dds.docs.eprosima.com/en/latest/02-formalia/titlepage.html) and [Getting Started](https://fast-dds.docs.eprosima.com/en/latest/fastdds/getting_started/getting_started.html) sections of the FastDDS documentation.

## What is a plugin?
One of the main feature of DLS2 is the ability to plug and unplug a new software at run-time into the DLS2 network. This ensures modularity, scalability and trasparency.

![Alt text](images/plug_and_play.png)

Different software module can have different inputs and outputs, so each module has its own plugin. A plugin is a component attached to the DLS2 network that:
* reads the module inputs from the DLS2 middleware
* write the module outputs in the DLS2 middleware

In this way, the implementation of the software module can be independent from the DLS2 middleware. The user can therefore mainly focus on the development of its own software module, being agnostic with respect to the communication channel used to receive inputs and to send outputs.

We can have periodic plugin, running in real-time at a specified frequency, or aperiodic plugin, such as services.

### Periodic app plugin
#### PeriodicAppPlugin class overview
The PeriodicAppPlugin class is the interface that allows a software module to run periodically, while communicating with the DLS2 network and it runs with a real-time scheduling policy. The plugin of each module is implemented as a class inheriting from the PeriodicAppPlugin. This class allows the specific plugin to:
* be loaded at runtime, through the *create_t* function
* add inputs to the plugin, with the *buildInput* function 
* add outputs to the plugin, with the *buildOutput* function
* define tasks (like initialization) to perform when the activation request is received, through the _checkActivation_ function
* run periodically the *run* function
* define what to do when closing (_unloading_) the plugin by means of _close()_ function
* read/write all the inputs/outputs from/to the DLS2 network

The *create_t*, *destroy_t* and *run* functions has to be implemented in any class inheriting from PeriodicAppPlugin. See [How to write a periodic plugin](#how-to-write-a-periodic-plugin) section.

#### How to write a periodic plugin
In this section it is described which are the components that a specific plugin must have and how to implement them. The [Overview](#overview) section describes the components. The [Steps](#steps) section instead, tells you how to create a plugin, by following what it is written in the overview. So you could directly go to the [Steps](#steps) section, despite it is kindly suggested to read the overview.

##### Overview
The connection between a module to be run periodically and its Plugin is depicted in the following figure 

![Alt text](images/plugin_module.png)

The basic idea is that the Plugin calls in its run function the run function of the module, allowing to:
* set inputs to the module, read from the DLS2 network
* get outputs of the module, writing them in the DLS2 network

The specific Plugin of the module has to be a C++ class inheriting from PeriodicAppPlugin and it is at the end a shared library. In this way, to run the software module at an arbitrary rate, you need to load from the command line its associated Plugin. With **loading** you are actually **spawning a new process** that will run your module. This process is following [this](https://github.com/iit-DLSLab/dls2/tree/main/modules/application#state-machine) state machine. To be loaded at run-time, the Plugin has to implement the *create_t* fuction, inherited from PeriodicAppPlugin class. This function is called when loading the Plugin at run-time, and it is responsible to call the Plugin constructor. Another function, *destroy_t*, has to be implemented too, to call the Plugin destructor once its instance is destroyed.

The Plugin has to store internally the following main objects:
* module object: this is an instance of your module class
* readers: they store a msg variable updated with the last read message when calling the *read()* function
* writers: they store a msg variable, that can be updated such that the message can be published to the DLS2 network when calling the *write()* function (the timestamp is already filled by *write()*)

Once it is loaded, the Plugin constructor has to:
* initialize the module
* define the inputs and outputs (i.e. readers and writers)

After the construction, the Plugin goes in the _idle_ state and the process holding it falls asleep waiting for an activation request. Once it is received, the _checkActivation()_ function is run periodically until a timeout: this is the function where you can initialize your module and do some basic checks. If the activation fails, the process goes back to the _idle_ state, otherwise the process starts executing its run function periodically, which has to call three main functions: 
* *read()*: it gets all the inputs of the module from the DLS2 network
* *module.run(inputs, outputs)*: it calls the run function of the module, which is responsible for implementing the module logic. Here *module* is the instace of your module class
* *write()*: it sends all the outputs of the module to the DLS2 network

From this structure, it easy to understand that the only compatibility requirement a module has to have to communicate with the DLS2 network is to define a *run(input,outputs)* function, whose arguments are the inputs and the outputs of the module.

##### What is an input?
In the Plugin an input is identified by:
* a topic to read from
* a reader object, listening on the topic, and built by the *buildInput* function

##### What is an output?
In the Plugin an output is identified by:
* a topic to write to
* a writer object, publishing on the topic, and built by the *buildOutput* function

##### Custom console commands
To interact with your module through the DLS2 console, you can define in the plugin class custom console commands.

##### Steps
From the organization point of view, for each plugin there is a repo.
To create a periodic plugin:
* [create an empty repo on GitHub](https://github.com/organizations/iit-DLSLab/repositories/new)
* pull the repo you have just created in the folder shared with the dls2 docker image (any subfolder of it is also fine)
* open the dls2 docker image
* build and install dls2_deploy
* go inside the repo you have just pulled
* call the following command for C++ plugins

        create_periodic_plugin

    that requests you to provide
    * a Plugin Type, that can be any of these
        * hardwares
        * controllers
        * estimators
        * motion_generations
    * a Plugin name. This will be automatically also the name of your module

    This command will create in the current directory (so in your repo) a basic project structure with the following main folders:
    * module: where you develop your module
    * plugin: where you develop the plugin
  
* call the following command for Python plugins

        create_py_periodic_plugin

    that requests you to provide
    * a Plugin name. This will be automatically also the name of your module
    * a list of types to be used by your python node (e.g. BlindState, BaseState, ...).

    This command will create in the current directory (so in your repo) a basic project structure with the following main folders:
    * plugin: where you store scheduler configs 
    * python: where you develop the plugin
    
    The Plugin class is automatically created, together with some suggestions on how to customize your plugin. 
    
    In order to launch the python nodes be sure to
    - compile and install the package (e.g. you can add in the main CMakeLists.txt as a sub-directory)
    - append the node name in `python_periodic_apps` in the startup file.

    IMPORTANT NOTE: when modifying the python source code, be sure to run again `sudo make install` before executing again.
    
    An example of Python plugin is [dummy_py_node](python/test/dummy_py_node)

    Moreover, in the project structure there is also the possibility to define custom messages and topics, that you can use in your plugin and made available to the DLS2 network.
* follow the instruction in the README of the project you have just created. You can find the README also [here](https://github.com/iit-DLSLab/dls2/tree/main/modules/plugin/skeletons/periodic#periodic-plugin).

### App plugin class overview
TODO
