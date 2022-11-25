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
	Service<req_pubsub_t, res_pubsub_t>::Service(const dls::topicType &topic_, callback_t callback_)
		: service_topic(topic_)
		, request_subscriber(
			"request_sub",
			dls::domains::services,
			topic_,
			std::function<void(void *)>{[&](void* tuple)
			{
				// ============= read the request message ==============
				typename req_pubsub_t::type request = *((typename req_pubsub_t::type*) tuple);

				std::stringstream out_topic_stream;
				out_topic_stream << std::get<0>(this->service_topic) << "_response_";

				// ============== Process the request ==============
				if constexpr (std::is_same<typename res_pubsub_t::type, void>() == false)
				{
					typename res_pubsub_t::type response = this->callback(request);

					dls::DDSWriter response_publisher(
						"response_pub",
						dls::domains::services,
							dls::topicType(out_topic_stream.str(), new req_pubsub_t())
					);

					// =============== Send the response ===============
					response_publisher.sendMessage((void*) &response);
				}
			}})
		, callback(callback_)
	{ }

	// =========================================================================
	// Service Client Implementation
	// =========================================================================
	template <typename req_pubsub_t, typename res_pubsub_t>
	ServiceClient<req_pubsub_t, res_pubsub_t>::ServiceClient(const dls::topicType &topic) 
		: response_mutex()
		, received_response_cv()
		, remote_response(nullptr)
		, request_publisher(
		 	"request_pub",
			dls::domains::services,
			topic)
		, response_subscriber(
			"response_sub",
			dls::domains::services,
			topic,
			std::function<void(void *)>{[&](void* tuple)
			{
				std::lock_guard<std::mutex> lock(this->response_mutex);

				if constexpr (std::is_same<typename res_pubsub_t::type, void>() == false)
				{
					typename res_pubsub_t::type response = *((typename res_pubsub_t::type*) tuple);
					this->remote_response = new typename res_pubsub_t::type(response);
				}
				this->received_response_cv.notify_all();
			}})
	{ }

	template <typename req_pubsub_t, typename res_pubsub_t>
	typename res_pubsub_t::type* ServiceClient<req_pubsub_t, res_pubsub_t>::call(typename req_pubsub_t::type &request, const std::chrono::duration<double> &duration)
	{
		std::unique_lock<std::mutex> lock(this->response_mutex);

		// send the request to the server
		this->request_publisher.sendMessage((void*) &request);

		// wait for a response
		this->received_response_cv.wait_for(lock, duration);

		// clean remote_response for the next call
		if constexpr (std::is_same<typename res_pubsub_t::type, void>() == false)
		{
			auto tmp = this->remote_response;
			this->remote_response = nullptr;
			return tmp;
		}

		return nullptr;		
	}

	template <typename req_pubsub_t, typename res_pubsub_t>
	void ServiceClient<req_pubsub_t, res_pubsub_t>::call(
		typename req_pubsub_t::type &request, 
		std::function<void(typename req_pubsub_t::type)> callback,	
		const std::chrono::duration<double> &duration)
	{
		std::thread t([&]()
		{
			typename res_pubsub_t::type response = this->call(request, duration);
			callback(response);
		});

		t.detach();
	}
} // end namespace dls

#endif /* end of include guard: SERVICE_TPP_DWLU8PQ9 */
