#include "dls/parameter_server.hpp"
#include "dls/parameter_store.hpp"

#include "dls2/msg/param_server_set_doublePubSubTypes.h"
#include "dls2/msg/doublePubSubTypes.h"
#include "dls2/msg/emptyPubSubTypes.h"
#include "dls2/msg/stringmsgPubSubTypes.h"

#include <mutex>
#include <string>

#define TOPIC_STRING(str) std::string("DLS_PARAMETER_SERVER_") + str

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

			dls::Service<ParamServerSetDoublePubSubType, EmptyMsgPubSubType> add_double;
			dls::Service<StringMsgPubSubType, DoubleMsgPubSubType> get_double;

			// dls::Service<DoubleMsgPubSubType, EmptyMsgPubSubType> add_double;
			// dls::Service<ParameterServerRequest<ParamServerSetDouble>, void> add_double;
			// dls::Service<ParamServerSetDouble, ParamServerSetDouble> add_double;
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
				[&](ParamServerSetDouble msg) -> EmptyMsg
				{
					std::lock_guard<std::mutex> lock(this->param_store_mutex);
					this->param_store.add(msg.key(), msg.value());
					return EmptyMsg();
				}
			),
			get_double
			(
				TOPIC_STRING("get_double"),
				[&](StringMsg msg) -> DoubleMsg
				{
					std::lock_guard<std::mutex> lock(this->param_store_mutex);
					auto ret = this->param_store.get<double>(msg.msg());
					double return_val = 0;
					if(ret)
					{
						return_val = ret.get();
					}
					DoubleMsg return_message;
					return_message.val() = return_val;
					return return_message;
				}
			)//,
			// add_int
			// (
			// 	TOPIC_STRING("add_int"),
			// 	[&](ParmeterServerRequest<int> &req)
			// 	{
			// 		std::lock_guard<std::mutex> lock(this->param_store_mutex);
			// 		this->param_store.add(req.key, req.data);
			// 		return 1;
			// 	}
			// ),
			// get_int
			// (
			// 	TOPIC_STRING("get_int"),
			// 	[&](std::string &req)
			// 	{
			// 		std::lock_guard<std::mutex> lock(this->param_store_mutex);
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
