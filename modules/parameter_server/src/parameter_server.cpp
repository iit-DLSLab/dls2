#include "dls/parameter_server.hpp"
#include "dls/parameter_store.hpp"
#include "dls2/util/service/service.hpp"

#include "dls2/msg/param_server_set_doublePubSubTypes.h"
#include "dls2/msg/doublePubSubTypes.h"
#include "dls2/msg/stringmsgPubSubTypes.h"

#include <mutex>
#include <string>

// namespace dls
// {

// 	// =========================== Pimpl Declaration ===========================
// 	namespace impl
// 	{
// 		template <typename T>
// 		struct ParameterServerRequest
// 		{
// 			std::string key;
// 			T data;
// 		};

// 		class ParameterServerImpl
// 		{
// 		public:
// 			ParameterServerImpl(std::string const & server_namespace);

// 		private:
// 			std::mutex param_store_mutex;
// 			dls::ParameterStore param_store;

// 			dls::Service<ParamServerSetDoublePubSubType>  add_double;
// 			dls::Service<StringMsgPubSubType> get_double;
// 		};

// 	}

// 	// ====================== Public class implementation ======================
// 	ParameterServer::ParameterServer(std::string const & ns) :
// 		pimpl(std::make_unique<impl::ParameterServerImpl>(ns))
// 	{ }

// 	// ========================= Pimpl Implementation ==========================
// 	namespace impl
// 	{
// 		ParameterServerImpl::ParameterServerImpl
// 		(
// 			std::string const &server_namespace
// 		) :
// 			param_store_mutex(),
// 			param_store(),
// 			add_double
// 			(
// 				dls::topicType("DLS_PARAMETER_SERVER_ADD_DOUBLE", new DoubleMsgPubSubType()),
// 				[this](ParamServerSetDouble msg) -> ParamServerSetDouble
// 				{
// 					std::lock_guard<std::mutex> lock(this->param_store_mutex);
// 					this->param_store.add(msg.key(), msg.value());
// 					return ParamServerSetDouble();
// 				}
// 			),
// 			get_double
// 			(
// 				dls::topicType("DLS_PARAMETER_SERVER_GET_DOUBLE", new DoubleMsgPubSubType()),
// 				[this](StringMsg msg) -> StringMsg
// 				{
// 					std::lock_guard<std::mutex> lock(this->param_store_mutex);
// 					auto ret = this->param_store.get<double>(msg.msg());
// 					double return_val = 0;
// 					if(ret)
// 					{
// 						return_val = ret.get();
// 					}
// 					StringMsg return_message;
// 					// return_message.val() = return_val;
// 					return return_message;
// 				}
// 			)
// 		{ }
// 	}
// }
