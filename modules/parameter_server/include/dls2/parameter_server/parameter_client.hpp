#ifndef PARAMETER_CLIENT_HPP
#define PARAMETER_CLIENT_HPP

#include <string>
#include <memory>

#include "dls2/service/service_client.hpp"

namespace dls
{
	class ParameterClient : public ServiceClient<ParamServerMsg, ParamServerMsg>
	{
	public:
		ParameterClient();
		~ParameterClient();
		void set(const std::string& key, const std::string& val);
		std::string get(const std::string& key);
	};
}

#endif // PARAMETER_CLIENT_HPP
