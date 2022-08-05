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
	template <typename msg_t>
	Service<msg_t>::Service
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
					// ============= read the request message ==============
					msg_t request = *((msg_t*) tuple);

					std::stringstream out_topic_stream;
					out_topic_stream << this->service_topic.first
									<< "_response_";

					// ============== Process the request ==============
					msg_t response = this->callback(request);

					dls::DDSWriter response_publisher(
						"response_pub",
						dls::domains::service,
						dls::topicType(out_topic_stream.str(), new msg_t())
					);

					// =============== Send the response ===============
					// return the response
					// this->response_publisher.publish(response);
					response_publisher.sendMessage((void*) &response);

				}
			}		
		),
		callback(callback_)
	{ }

	// =========================================================================
	// Service Client Implementation
	// =========================================================================
	template <typename msg_t>
	ServiceClient<msg_t>::ServiceClient
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
					msg_t response = *((msg_t*) tuple);

					std::lock_guard<std::mutex> lock(this->response_mutex);
					this->remote_response   = response;
					this->received_response = true;
					this->received_response_cv.notify_all();

				}
			}	
		),
		response_mutex(),
		received_response_cv(),
		remote_response(),
		received_response(false)
	{ }

	template <typename msg_t>
	msg_t* ServiceClient<msg_t>::call
	(
		msg_t &request,
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
			// reset state for next call
			this->received_response = false;

			return new msg_t(this->remote_response);
			
		}
		else
		{
			// indicate failure
			return nullptr;
		}
	}

	template <typename msg_t>
	bool ServiceClient<msg_t>::call
	(
		msg_t &request,
		std::function<void(msg_t response, bool success)> callback,
		const std::chrono::duration<double> &duration
	)
	{
		std::thread t
		(
			[&]()
			{
				bool success;
				msg_t response = this->call(request, duration);
				callback(response, success);
			}
		);

		t.detach();

		return true;
	}
} // end namespace dls

#endif /* end of include guard: SERVICE_TPP_DWLU8PQ9 */
