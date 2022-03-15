#include "dls2/util/service/service.hpp"
#include "parameter_server_topics.hpp"
#include "dls2/msg/param_server_set_doublePubSubTypes.h"
#include "dls2/msg/doublePubSubTypes.h"
#include "dls2/msg/emptyPubSubTypes.h"
#include "dls2/msg/stringmsgPubSubTypes.h"

#include "dls/parameter_server_client.hpp"

namespace servimpl = dls::parameter_server::impl;

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
			ParameterServerClientImpl(std::string const &server_namespace);

			dls::ServiceClient
			<
				ParamServerSetDoublePubSubType,
				DoubleMsgPubSubType
			> add_double;

			dls::ServiceClient
			<
				StringMsgPubSubType,
				DoubleMsgPubSubType
			> get_double;
		};

		// ========================== Implementation ===========================
		ParameterServerClientImpl::ParameterServerClientImpl
		(
			std::string const &server_namespace
		) :
			add_double
			(
				servimpl::buildFullTopicFromNamespace
				(
					server_namespace,
					servimpl::topics::add_double
				)
			),
			get_double
			(
				servimpl::buildFullTopicFromNamespace
				(
					server_namespace,
					servimpl::topics::get_double
				)
			)
		{ }
	}

	// =========================================================================
	// Main Class Implementation
	// =========================================================================
	// ============================= constructors ==============================
	ParameterServerClient::ParameterServerClient(std::string const & ns) :
		pimpl(std::make_unique<impl::ParameterServerClientImpl>(ns))
	{ }

	ParameterServerClient::~ParameterServerClient() { }

	// ============================= Param Server ==============================
	void ParameterServerClient::setDouble(std::string const &key, double d)
	{
		ParamServerSetDouble req;
		DoubleMsg            res;

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
