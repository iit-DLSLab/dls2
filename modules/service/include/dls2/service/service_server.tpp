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
#ifndef SERVICE_SERVER_TPP
#define SERVICE_SERVER_TPP

#include "dls2/service/service_server.hpp"

namespace dls
{
	template <typename req_pubsub_t, typename res_pubsub_t>
	ServiceServer<req_pubsub_t, res_pubsub_t>::ServiceServer(std::string& ID, const dls::topicType &topic_in_, const dls::topicType &topic_out_, std::function<void(void *, void *)> callback_)
	    : ServiceBase(ID)
		, service_topic_in(topic_in_)
        , service_topic_out(topic_out_)
		, request_subscriber(
			"request_sub",
			dls::domains::services,
			topic_in_,
			[&](void* tuple) -> void
			{
				// ============= read the request message ==============
				req_pubsub_t request = *((req_pubsub_t*) tuple);

				// ============== Process the request ==============
				res_pubsub_t response;
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
}

#endif /* end of include guard: SERVICE_SERVER_TPP */