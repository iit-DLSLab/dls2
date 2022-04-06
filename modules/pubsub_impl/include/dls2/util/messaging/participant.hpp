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

		template <class PubSub_t>
		bool addSubscriber();

	private:
		eprosima::fastdds::dds::DomainParticipant 	*participant;
		eprosima::fastdds::dds::TypeSupport 		type;

		std::vector<std::unique_ptr<dls::version2::SubscriberBase>> subscribers;

		// TBD - Implement PublisherBase
		//std::vector<std::unique_ptr<dls::version2::PublisherBase>>  publishers;

	};
	
} // namespace dls
/// \endcond

#endif // PARTICIPANT_HPP_4d198a8c_fdc4_4c7d_8be9_6d4b7e6bd7d2
