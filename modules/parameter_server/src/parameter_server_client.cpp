#include "dls2/util/service/service.hpp"

#include "dls2/parameter_server/parameter_server_client.hpp"

namespace dls
{
	// =========================================================================
	// Main Class Implementation
	// =========================================================================
	// ============================= constructors ==============================
	ParameterServerClient::ParameterServerClient() 
		: add_double(dls::topics::add_double)
		, get_double(dls::topics::get_double)
	{ }

	ParameterServerClient::~ParameterServerClient() { }

	// ============================= Param Server ==============================
	void ParameterServerClient::setDouble(std::string const &key, double d)
	{
		ParamSetMsg req;
		
		req.key()   = key;
		req.value() = d;

		this->add_double.call(req);
	}

	double ParameterServerClient::getDouble(std::string const &key)
	{
		StringMsg req;

		req.msg() = key;

		auto res = this->get_double.call(req);

		if(res)
		{
			return res->val();
		}

		return 0;
	}
}
