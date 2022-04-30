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
#ifndef SERVICE_TPP_DWLU8PQ9
#define SERVICE_TPP_DWLU8PQ9

#include "dls2/util/service/service.hpp"

// TODO temp include
#include <iostream>
#include <sstream>
#include <thread>

namespace dls
{
	// =========================================================================
	// Service Implementation
	// =========================================================================
	template <typename req_pubsub_t, typename res_pubsub_t>
	Service<req_pubsub_t, res_pubsub_t>::Service
	(
		const dls::topicType &topic_,
		callback_t callback_
	) :
		service_topic(topic_),
		request_subscriber
		(
			"request_sub",
			dls::domains::service,
			topic_,
			std::function<void(void *)>
			{
				[&](void* tuple)
				{
					// // ============= read the request message ==============
					// req_pubsub_t request = *((req_pubsub_t*) tuple);

					// std::stringstream out_topic_stream;
					// out_topic_stream << this->service_topic.first
					// 				<< "_response_";

					// // ============== Process the request ==============
					// res_pubsub_t response = this->callback(request);

					// dls::DDSWriter response_publisher(
					// 	"response_pub",
					// 	dls::domains::service,
					// 	dls::topicType(out_topic_stream.str(), new req_pubsub_t())
					// );

					// // =============== Send the response ===============
					// // return the response
					// // this->response_publisher.publish(response);
					// response_publisher.sendMessage((void*) &response);

				}
			}		
		),
		callback(callback_)
	{ }

	// =========================================================================
	// Service Client Implementation
	// =========================================================================
	template <typename req_pubsub_t, typename res_pubsub_t>
	ServiceClient<req_pubsub_t, res_pubsub_t>::ServiceClient
	(
		const dls::topicType &topic
	) :
		request_publisher(
			"request_pub",
			dls::domains::service,
			topic
		),
		response_subscriber(
			"response_sub",
			dls::domains::service,
			topic,
			std::function<void(void *)>
			{
				[&](void* tuple)
				{
					// res_pubsub_t response = *((res_pubsub_t*) tuple);

					// std::lock_guard<std::mutex> lock(this->response_mutex);
					// this->remote_response   = response;
					// this->received_response = true;
					// this->received_response_cv.notify_all();

				}
			}	
		),
		response_mutex(),
		received_response_cv(),
		remote_response(),
		received_response(false)
	{ }

	template <typename req_pubsub_t, typename res_pubsub_t>
	bool ServiceClient<req_pubsub_t, res_pubsub_t>::call
	(
		req_t &request,
		res_t *response,
		const std::chrono::duration<double> &duration
	)
	{
		std::unique_lock<std::mutex> lock(this->response_mutex);

		// send the request to the server
		this->request_publisher.sendMessage((void*) &request);

		// wait for a response
		this->received_response_cv.wait_for(lock, duration);

		if(this->received_response)
		{
			// place the response into the pointer supplied by the caller
			*response = this->remote_response;

			// reset state for next call
			this->received_response = false;

			// indicate success
			return true;
		}
		else
		{
			// indicate failure
			return false;
		}
	}

	template <typename req_pubsub_t, typename res_pubsub_t>
	bool ServiceClient<req_pubsub_t, res_pubsub_t>::call
	(
		req_t &request,
		std::function<void(res_t response, bool success)> callback,
		const std::chrono::duration<double> &duration
	)
	{
		std::thread t
		(
			[&]()
			{
				res_t response;
				bool success = this->call(request, &response, duration);
				callback(response, success);
			}
		);

		t.detach();

		return true;
	}
} // end namespace dls

#endif /* end of include guard: SERVICE_TPP_DWLU8PQ9 */
