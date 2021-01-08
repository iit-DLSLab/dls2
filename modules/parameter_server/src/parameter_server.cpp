#include "dls/parameter_server.hpp"
#include "dls/parameter_store.hpp"

#include "dls2/msg/param_server_serverPubSubTypes.h"

#include <mutex>
#include <string>

#define TOPIC_STRING(s) "DLS_PARAMETER_SERVER_" str

namespace dls
{

	// =========================== Pimpl Declaration ===========================
	namespace impl
	{
		template <typename T>
		struct ParameterServerRequest
		{
			std::string key;
			T data;
		};

		class ParameterServerImpl
		{
		public:
			ParameterServerImpl();
		private:
			std::mutex param_store_mutex;
			dls::ParameterStore param_store;

			dls::Service<ParameterServerRequest<double>, int>    add_double;
			// dls::Service<std::string,                    double> get_double;
			// dls::Service<ParameterServerRequest<int>,    int>    add_int;
			// dls::Service<std::string,                    int>    get_int;
		};

	}

	// ====================== Public class implementation ======================
	ParameterServer::ParameterServer() :
		pimpl(std::make_unique<impl::ParameterServerImpl>())
	{ }

	// ========================= Pimpl Implementation ==========================
	namespace impl
	{
		ParameterServerImpl::ParameterServerImpl() :
			param_store_mutex(),
			param_store(),
			add_double
			(
				TOPIC_STRING("add_double"),
				[&](ParmeterServerRequest<double> &req)
				{
					std::lock_guard<std::mute> lock(this->param_store_mutex);
					this->param_store.add(req.key, req.data);
					return 1;
				}
			)
			//,
			// get_double
			// (
			// 	TOPIC_STRING("get_double"),
			// 	[&](std::string &req)
			// 	{
			// 		std::lock_guard<std::mute> lock(this->param_store_mutex);
			// 		auto ret this->param_store.get(req);
			// 		if(ret)
			// 		{
			// 			return ret.get();
			// 		}
			// 		return 0;
			// 	}
			// ),
			// add_int
			// (
			// 	TOPIC_STRING("add_int"),
			// 	[&](ParmeterServerRequest<int> &req)
			// 	{
			// 		std::lock_guard<std::mute> lock(this->param_store_mutex);
			// 		this->param_store.add(req.key, req.data);
			// 		return 1;
			// 	}
			// ),
			// get_int
			// (
			// 	TOPIC_STRING("get_int"),
			// 	[&](std::string &req)
			// 	{
			// 		std::lock_guard<std::mute> lock(this->param_store_mutex);
			// 		auto ret this->param_store.get(req);
			// 		if(ret)
			// 		{
			// 			return ret.get();
			// 		}
			// 		return 0;
			// 	}
			// ),
		{ }
	}
}
