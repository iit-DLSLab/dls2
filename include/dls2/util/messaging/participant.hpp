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
#include <map>
#include <string>

namespace eprosima
{
	namespace fastrtps
	{
		class Participant;
	}
}

/// \cond doxygen_namespace_dls
namespace dls
{
	/// \cond doxygen_namespace_impl
	namespace impl
	{
		void initFastdds();

		auto getFastddsParticipant()
		    -> eprosima::fastdds::dds::DomainParticipant *;

		void closeFastdds();

		auto registerFastddsTopic(const std::string &topic_name,
		                          const std::string &rtps_type_name)
		    -> eprosima::fastdds::dds::Topic *;

		/// \cond doxygen_namespace_legacy
		namespace legacy
		{
			/// This function should be removed once the old publishers and
			/// subscribers are taken out
			auto getFastrtpsLegacyParticipant()
			    -> eprosima::fastrtps::Participant *;
		} /// \endcond namespace legacy

	} // namespace impl
	/// \endcond
} // namespace dls
/// \endcond

#endif // PARTICIPANT_HPP_4d198a8c_fdc4_4c7d_8be9_6d4b7e6bd7d2
