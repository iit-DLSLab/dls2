#include "hello_world_plugin.hpp"
#include "dls2/topics/topics.hpp"

HelloWorldPlugin::HelloWorldPlugin(const std::string& ID)
	: dls::PeriodicAppPlugin(ID)
{
	eprosima::fastdds::dds::DataReaderQos control_signal_qos(
        eprosima::fastdds::dds::DATAREADER_QOS_DEFAULT);

    const std::string value = "typehash=RIHS01_64087432a68cc5e76843ff92dc2d840ba6e26de892d2a307d7e90ad43ea29aab;";
    control_signal_qos.user_data().data_vec(std::vector<uint8_t>(value.begin(), value.end()));

    this->reader_cs.reset(new dls::Reader<dls2_interface::msg::ControlSignal>(
        this->dds_participant_,
        dls::topics::control_signal, 
        [this](){
            if (!this->reader_cs){
                std::cout << "[hello_world] reader_cs is null, cannot read message" << std::endl;
                return;
            }

            this->reader_cs->read();
            const auto& msg = this->reader_cs->msg;

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
