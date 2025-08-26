#include <chrono>
#include <cstdlib>
#include <iostream>
#include <vector>
#include <memory>
#include <thread>

#include "custom_plugin.hpp"
#include "dls2/topics/topics.hpp"

CustomPlugin::CustomPlugin(const std::string& ID)
	: dls::PeriodicAppPlugin(ID)
	, reader_bs(buildInput<BlindState>(dls::topics::low_level_estimation::blind_state, [](){}, false)) // false: not required on activation
	// , reader_bs(buildInput<BlindState>(dls::topics::low_level_estimation::blind_state)) // by default the input is required on activation
	, writer_cs(buildOutput<ControlSignal>(dls::topics::control_signal)){

	// Init outputs if there are variable size fields
	writer_cs->msg.torques().resize(12);
}

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

// Function for run-time dynamic loading through DLS2 console
extern "C" PeriodicAppPlugin *create(const std::string& ID){
	return new CustomPlugin(ID);
}
extern "C" void destroy(PeriodicAppPlugin *p){
		delete p;
}