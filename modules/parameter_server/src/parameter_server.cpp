#include "dls/parameter_server.hpp"
#include "dls/parameter_store.hpp"
#include "dls2/util/service/service.hpp"

#include "dls2/msg/param_server_set_doublePubSubTypes.h"
#include "dls2/msg/doublePubSubTypes.h"
#include "dls2/msg/stringmsgPubSubTypes.h"

#include "parameter_server_topics.hpp"

#include <mutex>
#include <string>

namespace servimpl = dls::parameter_server::impl;

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
			ParameterServerImpl(std::string const & server_namespace);

		private:
			std::mutex param_store_mutex;
			dls::ParameterStore param_store;

			dls::Service<ParamServerSetDoublePubSubType, DoubleMsgPubSubType>  add_double;
			dls::Service<StringMsgPubSubType,            DoubleMsgPubSubType> get_double;
		};

	}

	// ====================== Public class implementation ======================
	ParameterServer::ParameterServer(std::string const & ns) :
		pimpl(std::make_unique<impl::ParameterServerImpl>(ns))
	{ }

	// ========================= Pimpl Implementation ==========================
	namespace impl
	{
		ParameterServerImpl::ParameterServerImpl
		(
			std::string const &server_namespace
		) :
			param_store_mutex(),
			param_store(),
			add_double
			(
				servimpl::buildFullTopicFromNamespace
				(
					server_namespace,
					servimpl::topics::add_double
				),
				[this](ParamServerSetDouble msg) -> DoubleMsg
				{
					std::lock_guard<std::mutex> lock(this->param_store_mutex);
					this->param_store.add(msg.key(), msg.value());
					return DoubleMsg();
				}
			),
			get_double
			(
				servimpl::buildFullTopicFromNamespace
				(
					server_namespace,
					servimpl::topics::get_double
				),
				[this](StringMsg msg) -> DoubleMsg
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
			)
		{ }
	}
}
