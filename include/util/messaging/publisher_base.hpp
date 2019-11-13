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
********************************************************************************
* Author:            Hendrik de Bruin                                          *
* Maintainer:        Hendrik de Bruin                                          *
* author email:      hendrik.debruin@iit.it                                    *
*******************************************************************************/
#ifndef PUBLISHER_BASE_HPP_MFE9PIJK
#define PUBLISHER_BASE_HPP_MFE9PIJK

#include <memory>
#include <fastrtps/fastrtps_fwd.h>
#include <fastrtps/publisher/PublisherListener.h>

template <class PubSub_t>
class PublisherBase : public eprosima::fastrtps::PublisherListener
{
public:
	PublisherBase(const std::string &topic);
	virtual ~PublisherBase() = default;

	void publish(typename PubSub_t::type &msg) const;

private:
	std::shared_ptr<eprosima::fastrtps::Participant> pParticipant;
	std::shared_ptr<eprosima::fastrtps::Publisher> pPublisher;

	static PubSub_t rtps_type;
};

#include "util/messaging/publisher_base.tpp"

#endif /* end of include guard: PUBLISHER_BASE_HPP_MFE9PIJK */
