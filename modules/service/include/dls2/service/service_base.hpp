
#ifndef SERVICE_BASE_HPP
#define SERVICE_BASE_HPP

#include "dls2/application/app.hpp"

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
	class ServiceBase : public App
	{
	public:
		
		typedef ServiceBase *create_t(std::string);
        typedef void destroy_t(ServiceBase*);

		/// Creates a service
		///
		/// @param topic    the topic on which this service listens for requests
		/// @param callback a pointer to a callback function that is called when
		///                 this service receives a request
		ServiceBase(std::string& ID);

		virtual ~ServiceBase() = default;
		
		AppStatus run() override;

        void close() override;

		/// Print the state of this layer
		///
		virtual std::string where() {return "Parameter server";}

        /// Emergency stop for this component
        ///
        AppStatus eStop() override {return this->getStatus();};

	private:
		
		bool should_quit;

	};

	
} // end namespace dls

#endif /* end of include guard: SERVICE_BASE_HPP */