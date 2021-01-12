#include "dls2/util/service/service.hpp"
#include "parameter_server_topics.hpp"
#include "dls2/msg/param_server_set_doublePubSubTypes.h"
#include "dls2/msg/doublePubSubTypes.h"
#include "dls2/msg/emptyPubSubTypes.h"
#include "dls2/msg/stringmsgPubSubTypes.h"

#include "dls/parameter_server_client.hpp"

namespace dls
{
	// =========================================================================
	// Private Implementation
	// =========================================================================
	namespace impl
	{
		// ============================ Declaration ============================
		struct ParameterServerClientImpl
		{
			ParameterServerClientImpl();
			dls::ServiceClient<ParamServerSetDoublePubSubType, EmptyMsgPubSubType>  add_double;
			dls::ServiceClient<StringMsgPubSubType,            DoubleMsgPubSubType> get_double;
		};

		// ========================== Implementation ===========================
		ParameterServerClientImpl::ParameterServerClientImpl() :
			add_double(dls::parameter_server::impl::topics::add_double),
			get_double(dls::parameter_server::impl::topics::get_double)
		{ }
	}

	// =========================================================================
	// Main Class Implementation
	// =========================================================================
	// ============================= constructors ==============================
	ParameterServerClient::ParameterServerClient() :
		pimpl(std::make_unique<impl::ParameterServerClientImpl>())
	{ }

	ParameterServerClient::~ParameterServerClient() { }

	// ============================= Param Server ==============================
	void ParameterServerClient::setDouble(std::string const &key, double d)
	{
		ParamServerSetDouble req;
		EmptyMsg             res;

		req.key()   = key;
		req.value() = d;

		this->pimpl->add_double.call(req, &res);
	}

	double ParameterServerClient::getDouble(std::string const &key)
	{
		StringMsg req;
		DoubleMsg res;

		req.msg() = key;

		if(this->pimpl->get_double.call(req, &res))
		{
			return res.val();
		}

		return 0;
	}
}
