#include "hello_world_plugin.hpp"
#include "dls2/topics/topics.hpp"

HelloWorldPlugin::HelloWorldPlugin(const std::string& ID)
	: dls::PeriodicAppPlugin(ID){
	reader_bs = buildInput<dls2_interface::msg::BlindState>(dls::topics::low_level_estimation::blind_state, [](){}, false); // false: not required on activation
	writer_cs = buildOutput<dls2_interface::msg::ControlSignal>(dls::topics::control_signal);
	writer_cs->msg.joints_torques().resize(12);
	writer_cs->msg.control_mode() = static_cast<uint8_t>(ControlMode::TORQUE_MODE);

	//define console functions here if needed
	command_manager.addCommand("set_joint_torque",
                                        "Set joint torque",
                                        &HelloWorldPlugin::setJointTorque, this, {}, true);
}

HelloWorldPlugin::~HelloWorldPlugin(){}

void HelloWorldPlugin::run(const std::chrono::system_clock::time_point &time){
	read();
 	
	for (size_t i=0; i<writer_cs->msg.joints_torques().size(); i++){
		writer_cs->msg.joints_torques()[i] += i;
	}

	std::cout << "[" << std::chrono::duration_cast<std::chrono::milliseconds>(time.time_since_epoch()).count() 
		<< "]: Received joint size " << reader_bs->msg.joints_position().size()  << "; publishing dummy joints_torques...\n"; 

	write();
}

bool HelloWorldPlugin::setJointTorque(){
	std::cout << "Choose the joint index\n";
	int idx{0};
	dls::CommandHelper::readValue<int>("Joint_ID", idx);
	double tau{0.0};
	if(dls::CommandHelper::readValue<double>("torque", tau, writer_cs->msg.joints_torques()[idx]))
	{
		writer_cs->msg.joints_torques()[idx] = tau;
	}
	return true;
}

// Function for run-time dynamic loading through DLS2 console
extern "C" PeriodicAppPlugin *create(const std::string& ID, const std::string& name){
	// avoid compilation warnings
	(void)name;
	return new HelloWorldPlugin(ID);
}
extern "C" void destroy(PeriodicAppPlugin *p){
		delete p;
}
