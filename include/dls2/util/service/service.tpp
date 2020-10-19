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
		const std::string &topic_,
		callback_t callback_
	) :
		service_topic(topic_),
		request_subscriber
		(
			topic_,
			// -----------------------------------------------------------------
			// Service request handling
			// -----------------------------------------------------------------
			// TODO perhapse move this lambda into a private member function for
			// legiblity
			[&](eprosima::fastrtps::Subscriber *sub)
			{
				// spawn a thread to handle the request
				//
				// this thread is immediately detached. As soon as the thread
				// exits, it is automatically cleaned up.
				// See man 3 pthread_detach
				std::thread t
				(
					[&]()
					{
						// ============= read the request message ==============
						req_t request;
						eprosima::fastrtps::SampleInfo_t info;
						if(sub->takeNextData((void*)&request, &info))
						{
							std::stringstream out_topic_stream;
							out_topic_stream << this->service_topic
											 << "_response_"
											 << info.sample_identity.writer_guid();

							// ============== Process the request ==============
							res_t response = this->callback(request);

							PublisherBase<res_pubsub_t> response_publisher
							(
								out_topic_stream.str()
							);

							// =============== Send the response ===============
							// return the response
							// this->response_publisher.publish(response);
							response_publisher.publish(response);

							// sleep this thread before removing it. Sleep is
							// required, since when publisher goes out of scope,
							// its history is removed. If this happens before
							// the client gets the response, it will never see
							// the message
							std::this_thread::sleep_for(std::chrono::seconds(10));
						}
					}
				);
				t.detach();
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
		const std::string &topic
	) :
		request_publisher(topic),
		response_subscriber
		(
			(
				// Build the topic
				//
				// request_publisher.getGuid can not be added to a string with
				// the addition operator, but it can be added to a stream using
				// the bitshift operator. This lambda just builds the topic name
				// that way and returns it imediately
				[&]()
				{
					std::stringstream ss;
					ss << topic
					<< "_response_"
					<< this->request_publisher.getGuid();

					return ss.str();
				}()
			),
			[&](res_t &response)
			{
				std::lock_guard<std::mutex> lock(this->response_mutex);
				this->remote_response   = response;
				this->received_response = true;
				this->received_response_cv.notify_all();
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
		this->request_publisher.publish(request);

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
