
#ifndef DDSREADER_CPP
#define DDSREADER_CPP

#include "dds_reader.hpp"
#include <chrono>

namespace dls
{
	DDSReader::DDSReader(
		std::string     partName_,
		dls::domainType domain_,
		std::string topicName,
		std::function<void(void *)> callback_,
		eprosima::fastdds::dds::DataReaderQos qos_
	)
		: DDSParticipant(partName_, domain_,eprosima::fastrtps::rtps::DiscoveryProtocol_t::SUPER_CLIENT)
	{
		
		if (callback_ != nullptr)
			// std::cout << " Adding the reader" << std::endl;
			sleep(1);
			// auto participant_vect  = this->getParticipants();

			// std::cout << "Getting participants: " << std::endl;

			// for(auto &t: participant_vect){

			// 	std::cout << "\t" << t << std::endl;
			// }
			
			// if (topicFound("blind_state"))
			// 	std::cout << "found blind_state topic" << std::endl;
			// 	// auto topic = getTopicFromString("blind_state");
			// 	// this->reader = this->addReader("unicReader", topic, callback_, qos_);

			// if (topicFound("desired_torques"))
			// 	std::cout << "found desired_torques topic" << std::endl;
			
			if (topicFound(topicName))
				std::cout << "found " <<  topicName << std::endl;	

			// auto topic = getTopicFromString("blind_state");
			this->reader = this->addReader("unicReader", topicName, callback_, qos_);


			std::cout << "End of discovered parts" << std::endl; 

			// this->addReader("unicReader", topic_, callback_, qos_);
	}

	DDSReader::~DDSReader(){
		this->reader = nullptr;
	}

} // end namespace dls

#endif /* end of include guard: DDSREADER_CPP */
