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
#ifndef SERVICE_HPP_ZNVNXYI5
#define SERVICE_HPP_ZNVNXYI5

// =============================================================================
// Includes
// =============================================================================
#include <condition_variable>
#include <chrono>
#include <mutex>

#include "dls2/util/messaging/dds_reader.hpp"
#include "dls2/util/messaging/dds_writer.hpp"

namespace dls
{
	// =========================================================================
	// Service Server Class Declaration
	// =========================================================================
	/// A class representing a service that can be called asynchronously from
	/// external processes
	///
	/// \tparam req_pubsub_t the PubSubType corresponding to the request message
	/// that is sent to the server
	/// \tparam res_pubsub_t the PubSubType corresponding to the response
	/// message that is received from the server
	template <typename msg_t>
	class Service
	{
	public:
		/// The service callback signature
		typedef std::function<msg_t(msg_t)> callback_t;

		/// Creates a service
		///
		/// @param topic    the topic on which this service listens for requests
		/// @param callback a pointer to a callback function that is called when
		///                 this service receives a request
		Service
		(
			const dls::topicType &topic,
			callback_t callback
		);

	private:
		dls::topicType                      service_topic;

		/// Subscriber waiting for a request message
		///
		dls::DDSReader request_subscriber;

		/// The callback to call when a request is received
		///
		/// The return from this callback will automatically be sent to the
		/// client that made the request
		callback_t                       callback;

	};

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
	template <typename msg_t>
	class ServiceClient
	{
	public:
		/// Constructor
		///
		/// Creates a ServiceClient that can communicate with a server on the
		/// same topic.
		///
		/// @param topic the topic on which the server is listening for requests
		ServiceClient(const dls::topicType &topic);

		/// Calls the service
		///
		/// This call will block until the service responds. Optionally, a
		/// maximum blocking time can be specified.
		///
		/// @param request the request message to send to the service
		/// @param duration how long to wait for a response before giving up and
		///        returning early. Defaults to no time limit.
		/// @return true if the call was successful, false otherwise
		msg_t* call
		(
			msg_t &request,
			const std::chrono::duration<double> &duration =
				std::chrono::duration<double>
				{
					std::numeric_limits<double>::infinity()
				}
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
		///
		/// @return always true. This is to keep a similar interface to the
		///         other overload of `call`. Since the callback is handled
		///         asynchronously, it is not possible to determine in the
		///         return whether the call to the server was successful or not.
		///         The onus to handle failed calls rests with the user-defined
		///         callback passed into this function.
		bool call
		(
			msg_t &request,
			std::function<void(msg_t response, bool success)> callback,
			const std::chrono::duration<double> &duration =
				std::chrono::duration<double>
				{
					std::numeric_limits<double>::infinity()
				}
		);

	private:
		/// Publisher that sends the request to the server on the topic
		/// specified in the constructor
		// DO NOT put this after response_subscriber. response_subscriber
		// depends on request_publisher in the constructor initializer list
		dls::DDSWriter	request_publisher;

		/// Subscriber that receives the response from the server
		///
		dls::DDSReader response_subscriber;

		// BEGIN critical section
			std::mutex              response_mutex;
			std::condition_variable received_response_cv;
			msg_t                   remote_response;
			bool                    received_response;
		// END critical section
	};
} // end namespace dls

#include "dls2/util/service/service.tpp"

#endif /* end of include guard: SERVICE_HPP_ZNVNXYI5 */
