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
#ifndef SERVICE_CPP
#define SERVICE_CPP

#include "dls2/service/service.hpp"

namespace dls
{
	// =========================================================================
	// Service Implementation
	// =========================================================================
	Service::Service(std::string& ID_, const dls::topicType &topic_in_, const dls::topicType &topic_out_, std::function<void(void *, void *)> callback_)
	    : AppLayerComponent(ID_)
		, service_topic_in(topic_in_)
        , service_topic_out(topic_out_)
		, request_subscriber(
			"request_sub",
			dls::domains::services,
			topic_in_,
			[&](void* tuple) -> void
			{
				// ============= read the request message ==============
				decltype(service_topic_in.second) request = *((decltype(service_topic_in.second)*) tuple);

				// ============== Process the request ==============
				decltype(service_topic_out.second) response;
				this->callback(&request, &response);

                // =============== Send the response ===============
                response_publisher.sendMessage((void*) &response);
			})
		, response_publisher(
			"response_pub",
			dls::domains::services,
			topic_out_
		)
		, callback(callback_)
	{ }

    AppLayerComponent::Status Service::run()
	{
		return this->getStatus();
	}

	AppLayerComponent::Status Service::stop()
	{
		return this->getStatus();
	}
}

#endif /* end of include guard: SERVICE_CPP */