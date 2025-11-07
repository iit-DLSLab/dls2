#include "custom_plugin.hpp"
#include "dls2/topics/topics.hpp"

CustomPlugin::CustomPlugin(const std::string& ID)
	: dls::PeriodicAppPlugin(ID){
	reader_bs = buildInput<dls2_interface::msg::BlindState>(dls::topics::low_level_estimation::blind_state, [](){}, false); // false: not required on activation
	// reader_bs = buildInput<dls2_interface::msg::BlindState>(dls::topics::low_level_estimation::blind_state); // by default the input is required on activation
	writer_cs = buildOutput<dls2_interface::msg::ControlSignal>(dls::topics::control_signal);
	writer_cs->msg.torques().resize(12);

	//define console functions here if needed
	command_manager.addCommand("set_joint_torque",
                                        "Set joint torque",
                                        &CustomPlugin::setJointTorque, this, {}, true);
}

CustomPlugin::~CustomPlugin(){}

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

bool CustomPlugin::setJointTorque(){
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

// Function for run-time dynamic loading through DLS2 console
extern "C" PeriodicAppPlugin *create(const std::string& ID, const std::string& name){
	// avoid compilation warnings
	(void)name;
	return new CustomPlugin(ID);
}
extern "C" void destroy(PeriodicAppPlugin *p){
		delete p;
}