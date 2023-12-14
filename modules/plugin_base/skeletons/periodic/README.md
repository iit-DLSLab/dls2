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
In this section we will see how to customize the project. In the project files, there are comments that suggest you what to do. Some of them are straightforward. For others instead, it is provided an example to clarify what to do. The example considers that an user has created a plugin for a stance detection module.

### Outermost CMakeLists.txt
Let's start with the outermost CMakeLists.txt.

You can change the name of the plugin here

      # Project name
      project(stance_detection_plugin)
  This will be the name of the plugin library too. Be sure to end the name of the project with *_plugin*.

To change instead the module library name, modifying this variable

      # Module library name
      set(MODULE_LIBRARY_NAME stance_detection)
  Notice that the names of the other libraries are extracted from the module library name.

### Create the software module
In the module folder you can customize your software module.
####  module/CMakeLists.txt
In the module/CMakeLists.txt the module library is created. Here you can add other source files, include directories and libraries to be linked.

Notice that to include in the project external libraries to be compiled with yours, we can follow the convetion that they are put in the *module/thirty-parts* folder. For example, if ndcurves is an external library and you want to compile it each time, you will have 

      target_include_directories(${MODULE_LIBRARY_NAME}
            PUBLIC
                  include
            # add other include directories here
                  thirty-parties/ndcurves/include
      )

#### <module_name>.hpp and <module_name>.cpp
As we said, the module class has been already created. However, you can change the class according to your needs. In particular, in the <module_name>.hpp you have to:

* add the arguments of the module constuctor. For example

      StanceDetection(/*aguments_of_module_constructor*/);
  becames

      StanceDetection(std::shared_ptr<robotlib::RobotBase> pRobot);
  In this particular example, remember to add *#include "robotlib/robot_factory.hpp"*.
* add the inputs and outputs to the run function. For example

      void run(/*input_arguments, output_arguments*/);
  becames
  
      void run(robotlib::LegDataMap<bool> &stance_sensors_status, const Eigen::Matrix3d& w_R_b, const robotlib::JointState& q, const robotlib::JointState& qd, const robotlib::JointState& qdd, const robotlib::JointState& tau);
  Rember that the order of the arguments is: *inputs* THEN *outputs*. This inputs allows your module to be independent from the DLS2 messages and messages' wrappers.

  In the class it is also defined a YAML::Node, that it is used to read the module/config/config.yml file. In this file you can add configurations for your modules, for example

      aliengo_th:
            foot_LF_contact_force_th : 5
            foot_RF_contact_force_th : 5
            foot_LH_contact_force_th : 5
            foot_RH_contact_force_th : 5

The changes of the class constructor and run function signatures has to be imported in the module/src/<module_name>.cpp as well. In this file, you then have to provide the implementation of the run function. Remember that this is the function storing the "module logic", and it the one called periodically. 
### Create the plugin
In the plugin folder you can define the inputs and outputs of your module.
#### plugin/CMakeLists.txt
This file simply add the subdirectories: core, console_commands, messages and topics. By default the last two are commented, because a plugin could not need to create custom messages or plugins.

#### plugin/core
Here you define the plugin. The plugin stores internally an instance of both the module and console commands classes. To instantiate the module instance, you need to change the plugin constructor, adding the arguments of the module constructor. For example

      StanceDetectionPlugin (
            std::string &ID
            /*, aguments_of_module_constructor*/);
becames


      StanceDetectionPlugin(std::string& ID, const std::shared_ptr<robotlib::RobotBase> &robot);
                
This change has to be done as well in the plugin.cpp, by passing such arguments to the module constructor. For example

      StanceDetectionPlugin::StanceDetectionPlugin (std::string& ID/*, aguments_of_module_constructor*/) 
    : dls::PeriodicPluginBase(ID)
    , stance_detection(/*aguments_of_module_constructor*/) // instantiate module
becames
    
    StanceDetectionPlugin::StanceDetectionPlugin (std::string& ID, const std::shared_ptr<robotlib::RobotBase> &robot) 
    : dls::PeriodicPluginBase(ID)
    , stance_detection(robot) // instantiate module

The console commands instance is automatically instantiated.

Then, you have to define the inputs and the outputs. This is done in three steps:

* define member variables storing inputs and outputs
* define inputs and outputs topics
* build inputs and outputs, i.e., data reader and data writer, according to the topics

To define input/output variables:
* includes the headers of your inputs and outputs in plugin.hpp. For example

      //#include "dls2/msg_wrappers/msg_wrapper_name.hpp" // off-the-shelf wrapper
      //#include "estimators/stance_detection/msg_wrapper_name.hpp" // custom wrapper
  becames

      #include "dls2/msg_wrappers/blind_state.hpp" // input, off-the-shelf
      #include "dls2/msg_wrappers/base_state.hpp" //input, off-the-shelf
      #include "estimators/stance_detection/stance_status.hpp" //output, custom
  As you can see, the types of such variables correspond to a message wrapper. You can see also how to include either already provided messages, or custom ones.
* declare the variables in plugin.hpp. For example

      /*define_inputs*/
      /*define_output*/
  becames

      BlindState blind_state; //input
      BaseState base_state; //input
      StanceStatus stance_status; //output
* initialize the variables in plugin.cpp. For example

      /*, construct_input_variables*/ // instantiate input
      /*, construct_output_variables*/ //instantiate output

  becames

      , blind_state(robot)
      , base_state(robot)
      , stance_status(robot)
  Notice here that we are using the robot variable to create the wrappers. If your module does not need a robot object to be created, you still have to add the robot object to the plugin constructor, to be passed to the wrappers constructor. At the same time, there might be wrappers that does not need a robot object in their constructor.

To define the topics you can include them in the plugin.cpp. If you use already existing topics, decomment the follwing line

      //#include "dls2/topics/topics.hpp" // off-the-shelf topics
and in the plugin/core/CMakeLists.txt, add in the target_link_libraries command, under PUBLIC, the library *dls_topics*.

If you use instead custom topics, decomment

      //#include "estimators/stance_detection/topics.hpp" // custom topics
and in the plugin/core/CMakeLists.txt also decomment 
      
      #${TOPICS_LIBRARY_NAME}
You can of course have both custom and off-the-shelf topics.

You can now build the inputs and outputs. For example

      // Define inputs
        /*this->buildInput<message_wrapper_class, type_of_arguments_of_message_constructor>(
            topic_name,
            &input_variable_name,
            arguments_of_message_constructor
        );*/

        // Define outputs
        /*this->buildOutput<message_wrapper_class, type_of_arguments_of_message_constructor>(
            topic_name,
            &output_variable_name,
            arguments_of_message_constructor
        );*/
becames
        
        // Define inputs
        this->buildInput<BlindState, robot>(
            dls::topics::low_level_estimation::blind_state,
            &blind_state,
            robot
        );
        this->buildInput<BaseState, robot>(
            dls::topics::high_level_estimation::base_state,
            &base_state,
            robot
        );
        // Define outputs
        this->buildInput<StanceStatus, robot>(
            topics::stance_detection::stance_status,
            &stance_status,
            robot
        );

With this functions, we created the data readers and data writers of the plugin, associated to off-the-shelf and custom topics. Moreover, since we are passing the reference to our input/output variables when building the inputs/outputs:
 * the read() function automatically updates all the inputs
 * the write() function automatically take the output variables and publish them. It also automatically fills the timestamp, if the variable has one; so you do not need to bother about setting it

There are two last steps to be done:
* in the run function of the plugin, add the correct inputs and outputs to the run function of the module. For example 

      stance_detection.run(/*input_arguments, output_arguments*/);

  becames (according to the run function we have defined in the previous example)

      stance_detection.run(   blind_state.feet_contact_,
                              base_state.pose_.toRotationMatrix.transpose(),
                              blind_state.joints_position_,
                              blind_state.joints_velocity_,
                              blind_state.joints_acceleration_,
                              blind_state.joints_effort_
                              stance_status.stance_status_);
* modify the *PeriodicPluginBase \*create(std::string ID)* function. This function is called when the plugin is loaded at run-time, and it is responsible for the creation of a plugin instance, by calling the plugin constructor. For example, if the plugin takes as input a robotlib::RobotBase argument

      /*call_plugin_constructor*/
        return new StanceDetectionPlugin(ID/*, aguments_of_module_constructor*/);
  becames

      std::string robot_name="aliengo";

      if (robot_name == "")
      {
            std::string e = "Parameter robot_name is not defined, verify if the parameter server is running";
            throw std::runtime_error(e);
      }

      std::shared_ptr<robotlib::RobotBase> pRobot;
      try
      {
            pRobot = robotlib::RobotFactory::openRobot(robot_name);
      }
      catch (const std::exception &e)
      {
            std::cerr << "child_process: Could not open the robot " << robot_name << std::endl;
            std::cerr << e.what() << std::endl;
      }
      
      return new StanceDetectionPlugin(ID, pRobot);

  In this example the robot name is hardcoded, but in a future release this will not be needed anymore.

Congratulations! You have created your fist periodic plugin for dls2!

#### How to set the scheduler properties
In the plugin/config folder you have the possibility to set the properties of the scheduler.

### Create custom console commands
Creating console commmands is quite easy. The console commands are functions, called from the console, that change the status of your running module. Since the console is implemented as a DLS2 layer, to keep the module independent from how to interact with the console, a separated console function class is defined that links the plugin with the module. This is done by
* defining functions, to be used in the console, that changes the status of the running module
* adding such functions to the command manager of the plugin, as console commands

To do that:
* declare console functions in .hpp
* implement console functions in .cpp
* add the console functions to the command manager

### Create custom messages
// TODO

### Create custom topics


