
#ifndef DDSREADER_CPP
#define DDSREADER_CPP

#include "dls2/util/messaging/dds_reader.hpp"
#include <chrono>

namespace dls
{
	DDSReader::DDSReader(
		std::string     partName_,
		dls::domainType domain_,
		dls::topicType  topic_,
		std::function<void(void *)> callback_,
		eprosima::fastdds::dds::DataReaderQos qos_
	)
		: DDSParticipant(partName_, domain_,eprosima::fastrtps::rtps::DiscoveryProtocol_t::SUPER_CLIENT)
	{
		
		if (callback_ != nullptr)
			std::cout << " Adding the reader" << std::endl;
			sleep(1.5);
			auto participant_vect  = this->getParticipants();

			std::cout << "Getting participants: " << std::endl;

			for(auto &t: participant_vect){

				std::cout << t << std::endl;
			}

			if (discovery_database.find("blind_state") != discovery_database.end())
				std::cout << "found blind_state topic";

			if (discovery_database.find("pid") != discovery_database.end())
				std::cout << "found pid topic";	


			std::cout << "End of discovered parts" << std::endl; 

			this->reader = this->addReader("unicReader", topic_, callback_, qos_);
	}

	DDSReader::~DDSReader(){
		this->reader = nullptr;
	}

} // end namespace dls

#endif /* end of include guard: DDSREADER_CPP */
