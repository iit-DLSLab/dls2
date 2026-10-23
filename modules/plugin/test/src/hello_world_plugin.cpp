#include "hello_world_plugin.hpp"
#include "dls2/topics/topics.hpp"

HelloWorldPlugin::HelloWorldPlugin(const std::string& ID)
	: dls::PeriodicAppPlugin(ID)
{
	eprosima::fastdds::dds::DataReaderQos control_signal_qos(
        eprosima::fastdds::dds::DATAREADER_QOS_DEFAULT);

    const std::string value = "typehash=RIHS01_036168047cc117694951c8e71a72f5e84badb019d9d0f4a9c21f36110d48e1dd;";
    control_signal_qos.user_data().data_vec(std::vector<uint8_t>(value.begin(), value.end()));

    this->reader_cs.reset(new dls::Reader<dls2_interface::msg::ControlSignal>(
        this->dds_participant_,
        dls::topics::control_signal, 
        [](const dls2_interface::msg::ControlSignal& msg){
            std::cout << "ControlSignal timestamp: "
                      << msg.timestamp() << std::endl;
        }, 
        control_signal_qos));
}

HelloWorldPlugin::~HelloWorldPlugin(){}

void HelloWorldPlugin::run(const std::chrono::system_clock::time_point &time){
	// read();
	// write();
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
