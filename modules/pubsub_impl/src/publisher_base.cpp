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
#ifndef PUBLISHER_BASE_CPP
#define PUBLISHER_BASE_CPP

#include "dls2/util/messaging/publisher_base.hpp"

/// \cond doxygen_namespace_dls
namespace dls
{
	/// \cond doxygen_namespace_version2
	///
	/// Temporary namespace until the old publishers and subscribers are
	/// refactored into those contained here, then this namespace will be
	/// removed and its contents lifted to the dls namespace
	namespace version2
	{
		PublisherBase::PublisherBase ( eprosima::fastdds::dds::DomainParticipant *participant_) :
			participant(participant_)
		{}
	} /// \endcond namespace version2
} /// \endcond namespace dls
#endif /* end of include guard: PUBLISHER_BASE_CPP */
