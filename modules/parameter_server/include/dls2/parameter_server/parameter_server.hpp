#ifndef PARAMETER_SERVER_HPP
#define PARAMETER_SERVER_HPP

#include <memory>
#include <string>
#include <unordered_map>

#include "dls2/service/service.hpp"

namespace dls
{
	class ParameterServer : public dls::Service<ParamServerMsgPubSubType, DoubleMsgPubSubType> 
	{
	public:
		ParameterServer(std::string&);
		~ParameterServer();

	private:
        std::mutex param_store_mutex;
        std::unordered_map<std::string, double> param_store;
	};
}

#endif // PARAMETER_SERVER_HPP