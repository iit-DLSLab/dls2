#ifndef PARAMETER_SERVER_HPP_05f2fd08_3966_43ee_b7c8_5c344e7afd3f
#define PARAMETER_SERVER_HPP_05f2fd08_3966_43ee_b7c8_5c344e7afd3f

#include <memory>
#include <string>
#include <unordered_map>

#include "dls2/util/service/service.hpp"

namespace dls
{
	class ParameterServer
	{
	public:
		ParameterServer();

	private:
        std::mutex param_store_mutex;
        std::unordered_map<std::string, double> param_store;

        dls::Service<ParamSetMsgPubSubType>  add_double;
        dls::Service<StringMsgPubSubType, DoubleMsgPubSubType> get_double;

	};
}

#endif // PARAMETER_SERVER_HPP_05f2fd08_3966_43ee_b7c8_5c344e7afd3f