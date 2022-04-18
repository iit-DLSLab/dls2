/*******************************************************************************
*                                                       ,----,                 *
*                                                     .'   .' \                *
*                                                   ,----,'    |               *
*               ________  ___       ________        |    :  .  ;               *
*              |\   ___ \|\  \     |\   ____\       ;    |.'  /                *
*              \ \  \_|\ \ \  \    \ \  \___|_      `----'/  ;                 *
*               \ \  \ \\ \ \  \    \ \_____  \       /  ;  /                  *
*                \ \  \_\\ \ \  \____\|____|\  \     ;  /  /-,                 *
*                 \ \_______\ \_______\____\_\  \   /  /  /.`|                 *
*                  \|_______|\|_______|\_________\./__;      :                 *
*                                     \|_________||   :    .'                  *
*                                                 ;   | .'                     *
*                                                 `---'                        *
*******************************************************************************/
#ifndef PARTICIPANT_HPP_4d198a8c_fdc4_4c7d_8be9_6d4b7e6bd7d2
#define PARTICIPANT_HPP_4d198a8c_fdc4_4c7d_8be9_6d4b7e6bd7d2

#include <fastdds/dds/domain/DomainParticipantFactory.hpp>
#include <fastdds/dds/domain/DomainParticipant.hpp>

#include "dls2/util/messaging/subscriber.hpp"
#include "dls2/util/messaging/publisher.hpp"

#include "dls2/topics/topics.hpp"


#include <map>
#include <string>

/// \cond doxygen_namespace_dls
namespace dls
{
	class DDSParticipant{

	public:
		DDSParticipant(
			const std::string &partName_,
			const unsigned int &domain_
		);

		~DDSParticipant();

		std::vector<std::string> getParticipants();

		bool addWriter(
			std::pair<std::string, std::string> topic_
		);

		bool addReader(
			std::string 								topicName_,
			std::string 								dataType_,
			dls::version2::Subscriber::CallbackType		callback
		);

	private:
		eprosima::fastdds::dds::DomainParticipant 	*participant;
		eprosima::fastdds::dds::Topic				*topic;		

		dls::version2::Subscriber	*subscriber;
		dls::version2::Publisher	*publisher;

		bool addTopic( 
			std::string topicName_,
			std::string dataType_
		);
	};
	
} // namespace dls
/// \endcond

#include "dls2/util/messaging/participant.tpp"

#endif // PARTICIPANT_HPP_4d198a8c_fdc4_4c7d_8be9_6d4b7e6bd7d2