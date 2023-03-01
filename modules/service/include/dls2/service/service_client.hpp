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
#ifndef SERVICE_CLIENT_HPP
#define SERVICE_CLIENT_HPP

// =============================================================================
// Includes
// =============================================================================
#include <condition_variable>
#include <chrono>
#include <mutex>

#include "dls2/components/periodic_app_layer_component.hpp"

#include "dls2/util/messaging/dds_reader.hpp"
#include "dls2/util/messaging/dds_writer.hpp"

namespace dls
{

	// =========================================================================
	// Service Client Class Declaration
	// =========================================================================
	/// A class that is able to communicate with a server running in the same or
	/// in a different process.
	///
	/// The client can call the server in a blocking, blocking with timeout,
	/// asynchronous or asynchronous with timeout fashion
	///
	/// \tparam req_pubsub_t the PubSubType corresponding to the request message
	/// that is sent to the server
	/// \tparam res_pubsub_t the PubSubType corresponding to the response
	/// message that is received from the server
	template <typename req_pubsub_t, typename res_pubsub_t>
	class ServiceClient
	{
	public:
		/// Constructor
		///
		/// Creates a ServiceClient that can communicate with a server on the
		/// same topic.
		///
		/// @param topic the topic on which the server is listening for requests
		ServiceClient(const dls::topicType &topic_req, const dls::topicType &topic_res);

		/// Calls the service
		///
		/// This call will block until the service responds. Optionally, a
		/// maximum blocking time can be specified.
		///
		/// @param request the request message to send to the service
		/// @return result value if the call was successful, nullptr otherwise
		void call
		(
			req_pubsub_t&request,
			res_pubsub_t &result
		);

		/// Calls the service
		///
		/// This call will block until the service responds. Optionally, a
		/// maximum blocking time can be specified.
		///
		/// @param request the request message to send to the service
		/// @param duration how long to wait for a response before giving up and
		///        returning early. Defaults to no time limit.
		/// @return result value if the call was successful, nullptr otherwise
		void call
		(
			req_pubsub_t&request,
			res_pubsub_t &result,
			const std::chrono::duration<double> &duration
		);

		/// Calls the service
		///
		/// The service response is handled inside an asynchronous user-defined
		/// callback that is run inside of a background thread. Optionally, a
		/// timeout may be specified. If the timeout expires before the server
		/// responds, then the client gives up.
		///
		/// @param request the request message to send to the service
		///
		/// @param callback a user-defined callback function that is called when
		///        the server responds, or when the optional timeout expires.
		///        The callback may be a function, a member function, a functor,
		///        a lambda or a std::function. The callable returns void. Its
		///        arguments are the following:
		///        * response: The response message type that is returned by the
		///          server.
		///        * success: A boolean which is set to true if the server was
		///          called successfully, else false if the optional timeout
		///          exired or the server could not be contacted.
		///
		/// @param duration how long to wait for a response before giving up and
		///        returning early. Defaults to no time limit.
		void call
		(
			req_pubsub_t &request,
			std::function<void(req_pubsub_t, res_pubsub_t)> callback,
			const std::chrono::duration<double> &duration =
				std::chrono::duration<double>
				{
					std::numeric_limits<double>::infinity()
				}
		);

	private:
		// BEGIN critical section
			std::mutex              		response_mutex;
			std::condition_variable 		received_response_cv;
			std::shared_ptr<res_pubsub_t>   remote_response;
			bool                    		received_response;
		// END critical section

		/// Publisher that sends the request to the server on the topic
		/// specified in the constructor
		// DO NOT put this after response_subscriber. response_subscriber
		// depends on request_publisher in the constructor initializer list
		dls::DDSWriter	request_publisher;

		/// Subscriber that receives the response from the server
		///
		dls::DDSReader response_subscriber;
	};
} // end namespace dls

#include "dls2/service/service_client.tpp"

#endif /* end of include guard: SERVICE_CLIENT_HPP */
