# Periodic plugin

## Introduction
This project skeleton represents the starting point of anyone that wants to create a C++ plugin to interface a software module with DLS2.

This project has been created with the command

        create_periodic_plugin

that requests you to provide a *Plugin Type* and a *Plugin Name* (which is also the name of the module).

The project structure is made by the following main folders:
* module: where you develop your module
* plugin: where you develop the plugin

The Plugin and module classes are automatically created, together with some suggestions on how to customize your plugin.

Notice that the include directories has a subdirectory following this convention

        include/<plugin_type>/<plugin_name>

where *\<plugin_name>* is the name of the plugin, and *\<plugin_type>* is the plugin type that can be: *hardwares, estimators, controllers, motion_generators*. This include structure helps with the inclusion of the headers in other libraries.

Don't worry, you are lucky: we will guide you step-by-step with the customization of your plugin!

## Build and install the project
Inside the dls2-framework docker image, to build and install the project you can

* cd <path_to_your_project>
* mkdir build
* cd build
* ccmake ..
* make
* sudo make install

With the `ccmake ..` command you can choose what to build and install:
* the software module (*<plugin_name>_module*)
* the plugin (*<plugin_name>_plugin*)

In this way, you can build and install separately each software part.

With the build and install steps, you have created the following libraries:
* *<plugin_name>*: plugin library
* *<plugin_name>_module*: module library

For example, when creating a *stance_detection* plugin, you will have:
* *stance_detection*
* *stance_detection_module*

Notice that the installation is a necessary step in order to load at run-time your plugin.

## Hands-on
In this section we will see how to customize the project. In the project files, there are comments that suggest you what to do. Some of them are straightforward. For others instead, it is provided an example to clarify what to do. Most of the examples consider that an user has created a plugin for a stance detection module. You will see that the procedure is longer in the explanation, but it is easy and fast in the implementation.

### Outermost CMakeLists.txt
Let's start with the outermost CMakeLists.txt.

You can change the name of the plugin here

      # set configuration variables
      set(PLUGIN_NAME <plugin_name>)

  This will be the name of the plugin library too.  Notice that the names of the other libraries are extracted from the plugin name.

### Create the software module
In the module folder you can customize your software module.
####  module/CMakeLists.txt
In the module/CMakeLists.txt the module library is created. Here you can add other source files, include directories and libraries to be linked.

Notice that to include in the project external libraries to be compiled with yours, we can follow the convetion that they are put in the *module/third-party* folder. For example, if ndcurves is an external library and you want to compile it each time, you will have 

      target_include_directories(${MODULE_LIBRARY_NAME}
            PUBLIC
                  include
            # add other include directories here
                  third-party/ndcurves/include
      )

#### <module_name>.hpp and <module_name>.cpp
As we said, the module class has been already created. However, you can change the class according to your needs. In particular, in the <module_name>.hpp you have to:

* add the arguments of the module constuctor. For example

      StanceDetection(/*aguments_of_module_constructor*/);
  becames

      StanceDetection(std::shared_ptr<robotlib::RobotBase> pRobot);
  In this particular example, remember to add *#include "robotlib/robot_factory.hpp"* and to link the *robotlib* library to the module library.
* add the inputs and outputs to the run function. For example

      void run(/*input_arguments, output_arguments*/);
  becames
  
      void run(robotlib::LimbDataMap<bool> &stance_sensors_status, const Eigen::Matrix3d& w_R_b, const robotlib::JointState& q, const robotlib::JointState& qd, const robotlib::JointState& qdd, const robotlib::JointState& tau);
  Rember that the order of the arguments is: *inputs* THEN *outputs*. Writing the run arguments in this way allows your module to be independent from the DLS2 messages.
* define the inputs of the init function (and its implementation) that it is called when activating the plugin. For example

      bool init(/*init_inputs*/);
  becames

      bool init( double period,
                    const robotlib::JointState& input_joints_position,
                    const dls::utils::Pose& input_pose,);
* In the class it is also defined a YAML::Node, that it is used to read the module/config/config.yaml file. In this file you can add configurations for your modules, for example

      aliengo_th:
            foot_LF_contact_force_th : 5
            foot_RF_contact_force_th : 5
            foot_LH_contact_force_th : 5
            foot_RH_contact_force_th : 5
* when choosing what to build and install with the *ccmake ..* command, set to *ON* the *<plugin_name>_module* option

The changes of the class constructor and run function signatures has to be imported in the module/src/<module_name>.cpp as well. In this file, you then have to provide the implementation of the run function. Remember that this is the function storing the "module logic", and it the one called periodically.
### Create the plugin
In the plugin folder you can define the inputs and outputs of your plugin.

#### plugin/core
Here you define the plugin. The plugin stores internally an instance of the module. To instantiate the module instance, you need to change the plugin constructor, adding the arguments of the module constructor. For example

      StanceDetectionPlugin (
            std::string &ID
            /*, aguments_of_module_constructor*/);
becames


      StanceDetectionPlugin(std::string& ID, const std::shared_ptr<robotlib::RobotBase> robot);
                
This change has to be done as well in the plugin.cpp, by passing such arguments to the module constructor. For example

      StanceDetectionPlugin::StanceDetectionPlugin (std::string& ID/*, aguments_of_module_constructor*/) 
    : dls::PeriodicAppPlugin(ID)
    , stance_detection(/*aguments_of_module_constructor*/) // instantiate module
becames
    
    StanceDetectionPlugin::StanceDetectionPlugin (std::string& ID, const std::shared_ptr<robotlib::RobotBase> robot) 
    : dls::PeriodicAppPlugin(ID)
    , stance_detection(robot) // instantiate module

Notice that you need to change the *PeriodicAppPlugin \*create(const std::string& ID, const std::string& robot_name)* function too, according to the constructor arguments. This function is called when the plugin is loaded at run-time, and it is responsible for the creation of a plugin instance, by calling the plugin constructor. For example, if the plugin takes as input a robotlib::RobotBase argument

      /*call_plugin_constructor*/
        return new StanceDetectionPlugin(ID/*, aguments_of_module_constructor*/);
  becames

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

Now, you have to define the inputs and the outputs. This is done by declaring and defining member variables storing data readers and data writers.

To declare and define data readers or data writers:
* includes the headers of the readers and writers

      // reader include
      #include "dls2/signal/reader.hpp"
      // writer include
      #include "dls2/signal/writer.hpp"

  and the messages you want to use, e.g.

      #include "dls_messages/dds/msg_name.hpp"

      becames

      #include <dls_messages/dds/blind_state.hpp> // input, off-the-shelf
      #include <dls_messages/dds/base_state.hpp> //input, off-the-shelf
      #include "estimators/stance_detection/stance_status.hpp" //output, custom

[Here](#create-custom-messages) you can see how to create a custom message.
* declare the readers/writers in plugin.hpp. For example

      /*define readers*/
      // dls::ReaderPtr<MsgType> reader_name;
      /*define writers*/
      // dls::WriterPtr<MsgType> writer_name;
  becames

	dls::ReaderPtr<dls2_interface::msg::BlindState> reader_bs;
	dls::WriterPtr<dls2_interface::msg::BaseState> writer_bs;
	dls::WriterPtr<StanceDetection> writer_sd;

* define the readers/writer in the constructor of the plugin (file plugin.cpp).
      For example

            // Define readers
            // reader_name = buildInput<MsgType>(<topic>);
            // Define writers
            // writer_name = buildOutput<MsgType>(<topic>);
      becames
            reader_bs = buildInput<dls2_interface::msg::BlindState>(dls::topics::low_level_estimation::blind_state);
            writer_bs = buildOutput<dls2_interface::msg::BaseState>(dls::topics::high_level_estimation::base_state);
            writer_sd = buildOutput<StanceDetection>(topics::stance_detection::stance_status);

  To include already existing topics, decomment the follwing line

      //#include <dls2/topics/topics.hpp> // off-the-shelf topics
  and in the plugin/core/CMakeLists.txt, add in the target_link_libraries command, under PUBLIC, the library *dls_topics*.

  If you use instead custom topics, you have to include your topics.hpp file and change the plugin/core/CMakeLists.txt. For example, you need to decomment

      //#include "estimators/stance_detection/topics.hpp" // custom topics
  and in the plugin/core/CMakeLists.txt also decomment 
      
      #${TOPICS_LIBRARY_NAME}

Notice that:
* the read() function automatically updates all the inputs. Each reader has a msg member variable storing the read message
* the write() function automatically take the output variables and publish them. It also automatically fills the timestamp, if the variable has one; so you do not need to bother about setting it

There are few last steps to be done:
* in the run function of the plugin, add the correct inputs and outputs to the run function of the module. For example 

      stance_detection.run(/*input_arguments, output_arguments*/);

  becames (according to the run function we have defined in the previous example)

      stance_detection.run(   reader_bs->msg.feet_contact(),
                              reader_bs->msg.joints_position(),
                              writer_sd->msg.stance_status());
* _checkActivation_ is the function called when activating the plugin. If you want to initialize your module when activating the plugin, you can call the _init_ function of the module inside _checkActivation_. For example

      bool PeriodicGeneratorPlugin::checkActivation()
      {
            if(basicActivationChecks())
            {
                  return periodic_generator.init(/*init_inputs*/);
            }
            return false;
      }
  becames

      bool PeriodicGeneratorPlugin::checkActivation()
      {
            if(basicActivationChecks())
            {
                  read();
                  return periodic_generator.init(
                              input_blind_state.joints_position,
                              input_base_state.pose_,);
            }
            return false;
        }
* when deactivating the plugin, the _deactivation_ function is called periodically instead of the run function. In this case, you can customize what to do when deactivating. For example

      bool PeriodicGeneratorPlugin::deactivation(const std::chrono::system_clock::time_point& time){
            return true;
      }

  becames

      if(!periodic_generator.isGeneratingTrajectory()){
            return true;
      }
      else{
      if(!periodic_generator.isStopRequested())
            periodic_generator.stopMotion();

      run(time);

      if(periodic_generator.isStopped())
            return true;
      else
            return false;
      }

* when choosing what to build and install with the *ccmake ..* command, set to *ON* the *<plugin_name>_plugin/core* option. If you are using custom messages and/or custom topics set to *ON*, respectively, *<plugin_name>_plugin/messages* and *<plugin_name>_plugin/topics* options.

Congratulations! You have created your fist periodic plugin for dls2!

#### How to set the scheduler properties
In the plugin/config folder you have the possibility to set the properties of the scheduler.

### Create custom console commands
Creating console commmands in plugin/console_commands is quite easy. The console commands are functions, called from the console, that change the status of your running module. To add a console command:
* define the function to be called from the console in plugin.hpp. E.g.

		// console commands definitions
		/* bool function_name();*/

      becomes

            // console commands definitions
		bool setJointTorque()
* register the function to the command manager in the constructor of the plugin (file plugin.cpp). E.g

		//define console functions here
		command_manager.addCommand("function_name",
                                          "Description",
                                          &HelloWorldPlugin::function_name, this, {}, true);
      becomes

            //define console functions here
            command_manager.addCommand("set_joint_torque",
                                          "Set joint torque",
                                          &HelloWorldPlugin::setJointTorque, this, {}, true);

* implement the console functions in plugin.cpp. For example

	// bool HelloWorldPlugin::function_name(){
	// 	return true;
	// }

  becames

      bool HelloWorldPlugin::setJointTorque(){
            std::cout << "Choose the joint index\n";
            int idx{0};
            dls::CommandHelper::readValue<int>("Joint_ID", idx);
            double tau{0.0};
            if(dls::CommandHelper::readValue<double>("torque", tau, writer_cs->msg.torques()[idx]))
            {
                  writer_cs->msg.torques()[idx] = tau;
            }
            return true;
      }

  As you can see from this example, to get the data from the command line, you can use the *CommandHelper::readValue<value_type>* function. This function takes as inputs:
  
  * a comment to be displayed
  * the variable to be filled with the command line value; this variable is of *value_type* type which has to be equal to the command line value type
  * an optional value corresponding to the current value that you want to change. If it is provided, it is displayed; otherwise it is not

  It returns true if the console input is not empty.

* when choosing what to build and install with the *ccmake ..* command, set to *ON* the *<plugin_name>_plugin/console_comm* option

### Create custom messages
In plugin/messages you can define custom messages. To create a message:
* add its idl file in plugin/idls. For example, the *message.idl* file can be renamed to *stance_status.idl* and

      struct MessageName{};
  becames

      struct StanceStatusMsg
      {
            // Header
            string frame_id;
            unsigned long sequence_id;
            double timestamp;

            // Stance status
            double stance_status[4];
      };

* In plugin/CMakeLists.txt
  - decomment the following lines
      
      //# add custom messages
      // list(APPEND CMAKE_MODULE_PATH "/usr/include/dls_messages/cmake") # when using the installed dls2 project
      // list(APPEND CMAKE_MODULE_PATH  "${DLS_MESSAGE_FOLDER}/cmake") # when compiling together with the dls2 project
      // include(dls_message)		# message generation function
   - call the function *dls_add_message* with the message idl file name as argument. For example,

      dls_add_message(message ${MSGS_LIBRARY_NAME}) 	# generate message
    becames
 
      dls_add_message(stance_status) 	# generate message

### Create custom topics
To create a custom topic
* in plugin.hpp, declare the topic. For example

      // // header for implementing custom topics
      // #include "dls2/topics/utils.hpp"

      // // define topics here
      // namespace dls
      // {
      //     namespace topics
      //     {
      //         namespace HelloWorldPlugin{
      //             // extern dls::topicType topic_variable_name;
      //         }
      //     }
      // }
  becames
      // header for implementing custom topics
      #include "dls2/topics/utils.hpp"
      // define topics here
      namespace dls
      {
          namespace topics
          {
              namespace HelloWorldPlugin{
                  extern dls::topicType custom_topic;
              }
          }
      }
* in plugin.cpp, define the topic. This implies first of all including the [TypeSupport](https://fast-dds.docs.eprosima.com/en/latest/fastdds/dds_layer/topic/typeSupport/typeSupport.html?highlight=TopicDataType#definition-of-data-types) of each topic. Thanks to [Fast DDS-Gen](https://fast-dds.docs.eprosima.com/en/latest/fastdds/dds_layer/topic/fastddsgen/fastddsgen.html#fast-dds-gen-for-data-types-source-code-generation), the TypeSupport of each message is automatically created from the corresponding idl file, when building the project. For example, if you have a custom idl file in plugin/idls called stance_status.idl, you have that

      // topics
      // Include the TypeSupport of each message associated to each topic
      // #include <dls_messages/dds/<idl_file_name>PubSubTypes.hpp> // # off-the-shelf message
      // #include "dls_messages/dds/<idl_file_name>PubSubTypes.hpp" // # custom message

  becames
      
      // topics
      #include "dls_messages/dds/stance_statusPubSubTypes.hpp"
  
  Then you can define the topic. For example

      // namespace dls
      // {
      //     namespace topics
      //     {
      //         namespace HelloWorldPlugin{
      //             // dls::topicType topic_variable_name = dls::topicType("topic_name", new <message_name>PubSubType());
      //         }
      //     }
      // }

  becames

      namespace dls
      {
          namespace topics
          {
              namespace HelloWorldPlugin{
                  dls::topicType stance_status = dls::topicType("stance_status", new dls2_interface::msg::StanceStatusPubSubType());
              }
          }
      }

Notice that if you are creating a plugin of *controllers* type, to make the control layer using the output of the controller, the output topic should have the same name of the plugin library name. 

### Customize debian packaging
At the end of the outermost CMakeLists.txt, add the debian dependencies here

      set(CPACK_DEBIAN_PERIODIC_PACKAGE_DEPENDS       "dls2-runtime" CACHE INTERNAL "") # add here package dependencies

and here

      set(CPACK_DEBIAN_PERIODIC_DEV_PACKAGE_DEPENDS   "dls2-dev, dls-${PROJECT_NAME}" CACHE INTERNAL "") # add here package dependencies
For example you can have

      set(CPACK_DEBIAN_PERIODIC_PACKAGE_DEPENDS       "dls2-runtime, dls-state-estimator" CACHE INTERNAL "")
and

      set(CPACK_DEBIAN_PERIODIC_DEV_PACKAGE_DEPENDS   "dls2-dev, dls-${PROJECT_NAME}, dls-stance-detection, dls-state-estimator-dev" CACHE INTERNAL "")