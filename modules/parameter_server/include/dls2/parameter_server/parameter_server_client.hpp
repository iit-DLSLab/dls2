#ifndef PARAMETER_SERVER_CLIENT_HPP_a016e6d9_35ec_43c4_a2ae_78a5d05710f0
#define PARAMETER_SERVER_CLIENT_HPP_a016e6d9_35ec_43c4_a2ae_78a5d05710f0

#include <string>
#include <memory>

#include "dls2/util/service/service.hpp"

namespace dls
{
	class ParameterServerClient
	{
	public:
		ParameterServerClient();
		~ParameterServerClient();
		void   setDouble(std::string const &key, double val);
		double getDouble(std::string const &key);

	private:	
		dls::ServiceClient<ParamSetMsgPubSubType> add_double;
		dls::ServiceClient<StringMsgPubSubType, DoubleMsgPubSubType> get_double;
	};
}

#endif // PARAMETER_SERVER_CLIENT_HPP_a016e6d9_35ec_43c4_a2ae_78a5d05710f0
