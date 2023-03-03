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
#ifndef SERVICE_TPP
#define SERVICE_TPP

#include "dls2/service/service_client.hpp"

namespace dls
{
	// =========================================================================
	// Service Client Implementation
	// =========================================================================
	template <typename req_pubsub_t, typename res_pubsub_t>
	ServiceClient<req_pubsub_t, res_pubsub_t>::ServiceClient(const dls::topicType &topic_req, const dls::topicType &topic_res) 
		: response_mutex()
		, received_response_cv()
		, remote_response(nullptr)
		, request_publisher(
		 	"client_request_pub",
			dls::domains::services,
			topic_req)
		, response_subscriber(
			"client_response_sub",
			dls::domains::services,
			topic_res,
			[&](void* tuple)
			{
				std::lock_guard<std::mutex> lock(this->response_mutex);

				res_pubsub_t response = *((res_pubsub_t*) tuple);
				this->remote_response = std::make_shared<res_pubsub_t>(response);
				
				this->received_response_cv.notify_all();
			})
	{ }

	template <typename req_pubsub_t, typename res_pubsub_t>
	void ServiceClient<req_pubsub_t, res_pubsub_t>::call(
		req_pubsub_t &request, 
		res_pubsub_t &result,
		const std::chrono::duration<double> &duration)
	{
		// send the request to the server
		this->request_publisher.sendMessage((void*) &request);
		
		std::unique_lock<std::mutex> lock(this->response_mutex);

		// wait for a response
		this->received_response_cv.wait_for(lock, duration);

		// clean remote_response for the next call
		if(this->remote_response != nullptr)
		{
			result = *this->remote_response;
			this->remote_response.reset();
		}
	}

	template <typename req_pubsub_t, typename res_pubsub_t>
	void ServiceClient<req_pubsub_t, res_pubsub_t>::call(req_pubsub_t &request, res_pubsub_t &result)
	{
		// send the request to the server
		this->request_publisher.sendMessage((void*) &request);
		
		std::unique_lock<std::mutex> lock(this->response_mutex);

		// wait for a response
		this->received_response_cv.wait(lock);

		// clean remote_response for the next call
		if(this->remote_response != nullptr)
		{
			result = *this->remote_response;
			this->remote_response.reset();
		}
	}

	template <typename req_pubsub_t, typename res_pubsub_t>
	void ServiceClient<req_pubsub_t, res_pubsub_t>::call(
		req_pubsub_t &request, 
		std::function<void(req_pubsub_t, res_pubsub_t)> callback,	
		const std::chrono::duration<double> &duration)
	{
		std::thread t([&]()
		{
			res_pubsub_t response;
			this->call(request, response, duration);
			callback(response);
		});

		t.detach();
	}

} // end namespace dls

#endif /* end of include guard: SERVICE_TPP */