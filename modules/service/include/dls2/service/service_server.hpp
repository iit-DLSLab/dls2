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
#ifndef SERVICE_SERVER_HPP
#define SERVICE_SERVER_HPP

// =============================================================================
// Includes
// =============================================================================
#include <condition_variable>
#include <chrono>
#include <mutex>

#include "dls2/service/service_base.hpp"
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

	template <typename req_pubsub_t, typename res_pubsub_t>
	class ServiceServer : public ServiceBase
	{
	public:
		
		/// Creates a service server
		///
		/// @param topic    the topic on which this service listens for requests
		/// @param callback a pointer to a callback function that is called when
		///                 this service receives a request
		ServiceServer
		(
			std::string& ID,
			const dls::topicType& topic_in,
			const dls::topicType& topic_out,
			std::function<void(void *, void *)> callback
		);

		virtual ~ServiceServer() = default;
		
	private:
		
		dls::topicType service_topic_in;
		dls::topicType service_topic_out;
		/// Subscriber waiting for a request message
		///
		dls::DDSReader request_subscriber;
		dls::DDSWriter response_publisher;

		/// The callback to call when a request is received
		///
		/// The return from this callback will automatically be sent to the
		/// client that made the request
		std::function<void(void *, void*)> callback;

	};

} // end namespace dls

#include "dls2/service/service_server.tpp" 

#endif /* end of include guard: SERVICE_SERVER_HPP */