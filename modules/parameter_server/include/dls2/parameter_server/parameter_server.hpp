#ifndef PARAMETER_SERVER_HPP
#define PARAMETER_SERVER_HPP

#include <memory>
#include <string>
#include <unordered_map>

#include "dls2/service/service_server.hpp"
#include "dls2/command/command_manager.hpp"
#include "dls2/log/log.hpp"

namespace dls
{
	class ParameterServer : public dls::ServiceServer<dls2_interface::msg::ParamServer, dls2_interface::msg::ParamServer>
	{
	public:
		ParameterServer(std::string&);
		~ParameterServer();

	private:
        std::mutex param_store_mutex;
        std::unordered_map<std::string, std::string> param_store;

		CommandManager command_manager;
	};
}

#endif // PARAMETER_SERVER_HPP
