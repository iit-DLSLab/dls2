#include "dls2/util/service/service.hpp"
#include "dls_messages/dds/param_server_set_doublePubSubTypes.h"
#include "dls_messages/dds/doublePubSubTypes.h"
#include "dls_messages/dds/stringmsgPubSubTypes.h"

#include "dls2/parameter_server/parameter_server_client.hpp"

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

			dls::ServiceClient<ParamServerSetDoublePubSubType> add_double;
			dls::ServiceClient<StringMsgPubSubType> get_double;
		};

		// ========================== Implementation ===========================
		ParameterServerClientImpl::ParameterServerClientImpl
		(
			std::string const &server_namespace
		) :
			add_double
			(
				dls::topicType( server_namespace + "_ADD_DOUBLE", new ParamServerSetDoublePubSubType())
			),
			get_double
			(
				dls::topicType( server_namespace + "_GET_DOUBLE", new StringMsgPubSubType())
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
		ParamServerSetDouble *req = new(ParamServerSetDouble);
		
		req->key()   = key;
		req->value() = d;

		// req = this->pimpl->add_double.call(req);
	}

	double ParameterServerClient::getDouble(std::string const &key)
	{
		StringMsg req;

		req.msg() = key;

		// this->pimpl->get_double.call(req)

		// if()
		// {
		// 	return res.val();
		// }

		return 0;
	}
}
