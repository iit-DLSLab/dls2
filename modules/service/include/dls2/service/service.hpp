
#ifndef SERVICE_HPP
#define SERVICE_HPP

// =============================================================================
// Includes
// =============================================================================
#include <condition_variable>
#include <chrono>
#include <mutex>

#include "dls2/service/service_base.hpp"

namespace dls
{
	// =========================================================================
	// Service (Server) Class Declaration (with no request/response parameters)
	// =========================================================================
	/// A class representing a service that can be called asynchronously from
	/// external processes

	class Service : public ServiceBase
	{
	public:
		
		/// Creates a service (server)
		Service(std::string& ID);
		virtual ~Service() = default;
	};

} // end namespace dls

#endif // SERVICE_HPP