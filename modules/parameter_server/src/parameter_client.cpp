#include "dls2/parameter_server/parameter_client.hpp"

namespace dls
{
	ParameterClient::ParameterClient() 
		: ServiceClient(
			topics::param_server,
			dls::topicType(dls::topics::param_server.first + "_response", new dls2_interface::msg::ParamServerPubSubType())
		)
	{
		std::this_thread::sleep_for(std::chrono::milliseconds(1000));
	}

	ParameterClient::~ParameterClient()
	{}

	void ParameterClient::set(const std::string& key, const std::string& data)
	{
		dls2_interface::msg::ParamServer req;
		dls2_interface::msg::ParamServer res;
		
		req.key() = key;
		req.value() = data;
		req.rw() = true;

		this->call(req, res);
	}

	std::string ParameterClient::get(const std::string& key)
	{
		dls2_interface::msg::ParamServer req;
		dls2_interface::msg::ParamServer res;

		req.key() = key;
		req.rw() = false;

		this->call(req, res);

		return res.value();
	}
}
