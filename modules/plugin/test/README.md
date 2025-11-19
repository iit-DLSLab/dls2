# Description of the custom plugin example
A custom plugin can be created by inheriting from the PeriodicAppPlugin class, and defining the virtual `run` function. The structure is as follows:
- include/custom_plugin.hpp: declaration of the class
- src/custom_plung.cpp: definition of the class
- src/main.cpp: example of how to run the periodic plugin

## include/custom_plugin.hpp
    
    #include "dls2/plugin/periodic_app_plugin.hpp"
    
Inclusion of the PeriodicAppPlugin our class is going to inherit from.

    // -- readers/writers
    #include "dls2/signal/reader.hpp"
    #include "dls2/signal/writer.hpp"

Inclusion of the Reader and Writer class, used to define subscribers and publishers.


    // plugin loaded at run-time
    class CustomPlugin : public dls::PeriodicAppPlugin

Declaration of the custom class.

	CustomPlugin(const std::string& ID);
	
Class constructor declaration. It has an ID, but it can take further arbitrary arguments.

	virtual ~CustomPlugin();

Class destructorcdeclaration.

	void run(const std::chrono::system_clock::time_point &time) override;

This is the most important function. The `run` function is called periodically at run-time, and it is respobile for the execution of your task. The periodicity is defined in the `config/scheduler.yaml` file.

	// console commands
	bool setJointTorque();

Console command declaration that can be used from the DLS2 console.

	dls::ReaderPtr<dls2_interface::msg::BlindState> reader_bs;

Declaration of the input. This is a subscriber, using the BlinState message.

	dls::WriterPtr<dls2_interface::msg::ControlSignal> writer_cs;

Declaration of the output. This is apublisher, using the ControlSignal message.

## src/custom_plugin.cpp

    #include "custom_plugin.hpp"
    #include "dls2/topics/topics.hpp"

Inclusion of class and off-the-shelf topic declaration.

    CustomPlugin::CustomPlugin(const std::string& ID)
        : dls::PeriodicAppPlugin(ID){
        reader_bs = buildInput<dls2_interface::msg::BlindState>(dls::topics::low_level_estimation::blind_state, [](){}, false); // false: not required on activation
        // reader_bs = buildInput<dls2_interface::msg::BlindState>(dls::topics::low_level_estimation::blind_state); // by default the input is required on activation
        writer_cs = buildOutput<dls2_interface::msg::ControlSignal>(dls::topics::control_signal);

Initialization of the parent class and definition of the input and output.

    writer_cs->msg.torques().resize(12);

Initialization of the fields of the writer message(i.e. publisher) with variable size. This is necessary to then access easily with the proper size to this field.

	//define console functions here if needed
	command_manager.addCommand("set_joint_torque",
                                        "Set joint torque",
                                        &CustomPlugin::setJointTorque, this, {}, true);

Adding a custom function as console command.

    CustomPlugin::~CustomPlugin(){}

Destructore definition.

    void CustomPlugin::run(const std::chrono::system_clock::time_point &time){
        read();
        
        std::cout << "Time: " << std::chrono::duration_cast<std::chrono::milliseconds>(time.time_since_epoch()).count() << "ms, "<<"positions:\n"; 
        for (size_t i=0; i<reader_bs->msg.joints_position().size(); i++){
            std::cout <<reader_bs->msg.joints_position()[i] << " ";
        }
        std::cout << std::endl;
        for (size_t i=0; i<writer_cs->msg.torques().size(); i++){
            writer_cs->msg.torques()[i] += i;
        }

        write();
    }

Definition of the run function. This function has three main steps:

- inputs reading with the `read()` function
- task execution: dummy prints and torque setting
- outputs publishing with the `write()` function
```
// Function for run-time dynamic loading through DLS2 console
extern "C" PeriodicAppPlugin *create(const std::string& ID, const std::string& name){
    // avoid compilation warnings
    (void)name;
    return new CustomPlugin(ID);
}
extern "C" void destroy(PeriodicAppPlugin *p){
        delete p;
}
```

Finally, those two functions are usesd to dynamically load the plugin from the DLS2 console (see [dlopen](https://linux.die.net/man/3/dlopen)). In particular, in the `create` function the constructor of the custom class is called.

## src/main.cpp

    #include "custom_plugin.hpp"

Inclusion of the custom plugin

    int main()
    {
        CustomPlugin plugin("custom_plugin"); // same name of the library the CustomPlugin is compiled into

Initialization of the plugin. Notice that currently the ID of the plugin has to have the same name of the library the CustomPlugin is compiled into.

    plugin.execute();

Executing the plugin, which will follow the state machine defined [here](../../application/README.md).

# compile and install test by compiling dls2
# launch test from build directory as an executable
```
sudo ./../dls2/bin/Release/custom_plugin_test
```
sudo is used for setting RT priority to the process. Then activate the plugin from the dls2 console.
# launch test from dls2 console
```
loadPeriodicAppPlugin custom_plugin
```
Then activate the plugin from the dls2 console.