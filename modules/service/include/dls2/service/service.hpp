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
#ifndef SERVICE_HPP
#define SERVICE_HPP

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
	// Service Server Class Declaration
	// =========================================================================
	/// A class representing a service that can be called asynchronously from
	/// external processes
	///
	/// \tparam req_pubsub_t the PubSubType corresponding to the request message
	/// that is sent to the server
	/// \tparam res_pubsub_t the PubSubType corresponding to the response
	/// message that is received from the server
	class Service : public AppLayerComponent
	{
	public:
		
		typedef Service *create_t(std::string);
        typedef void destroy_t(Service*);

		/// Creates a service
		///
		/// @param topic    the topic on which this service listens for requests
		/// @param callback a pointer to a callback function that is called when
		///                 this service receives a request
		Service
		(
			std::string& ID,
			const dls::topicType& topic_in,
			const dls::topicType& topic_out,
			std::function<void(void *, void *)> callback
		);
		
		AppLayerComponent::Status run() override;

        AppLayerComponent::Status stop() override;

		/// Print the state of this layer
		///
		virtual std::string where() {return "Parameter server";}

        /// Emergency stop for this component
        ///
        Status eStop() override {return this->getStatus();};

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

		bool should_quit;

	};

	
} // end namespace dls

#endif /* end of include guard: SERVICE_HPP */