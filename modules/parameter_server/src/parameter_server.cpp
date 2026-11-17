#include "dls2/parameter_server/parameter_server.hpp"

#include <mutex>
#include <string>

namespace dls
{
	ParameterServer::ParameterServer(std::string& ID) 
		: ServiceServer(
			ID,
			dls::topics::param_server,
			dls::topicType(dls::topics::param_server.first + "_response", new dls2_interface::msg::ParamServerPubSubType()),
			[this](void* req, void* res) -> void
			{
				// std::lock_guard<std::mutex> lock(this->param_store_mutex);

				dls2_interface::msg::ParamServer* request_msg = (dls2_interface::msg::ParamServer*) req;
				dls2_interface::msg::ParamServer* result_msg = (dls2_interface::msg::ParamServer*) res;

				auto param = this->param_store.find(request_msg->key());

				// if it is a reading request and the parameter does not exists return 0
				if(param == this->param_store.end() && !request_msg->rw())
				{
					result_msg->value() = "";
					result_msg->rw() = false;
					return;
				}

				// update or insert the parameter if it is a writing request
				if(request_msg->rw())
				{
					param = param_store.emplace(request_msg->key(), request_msg->value()).first;
				}		

				// return the current value of the parameter
				result_msg->value() = param->second;
				result_msg->rw() = true;
				return;
			})
        , param_store_mutex()
        , param_store()
		, command_manager(ID)
    {

		command_manager.addCommand<>
		(
			"setParam",
			"Set a param in parameter server",
			std::function<bool(std::string key, std::string)>([&](std::string key, std::string data) -> bool
			{
				this->app_logger.info("Setting param: " + key + " with data: " + data);
				param_store.emplace(key, data);
				return true;
			}),
			{},
			true
		);

		command_manager.addCommand<>
		(
			"list",
			"List all the parameters",
			std::function<bool()>([&]() -> bool
			{
				for (auto elem : this->param_store)
				{
					std::cout << "key: " + elem.first + " data: " + elem.second << std::endl;
					this->app_logger.info("key: " + elem.first + " data: " + elem.second);
				}
				return true;
			}),
			{},
			true
		);


		this->app_logger.info("SERVICE " + ID + " IS RUNNING");
	}

	ParameterServer::~ParameterServer() 
	{
		this->app_logger.info("SERVICE " + this->getID() + " IS OFF");
	}

	// the class factories
    extern "C" ParameterServer* create(std::string ID) 
    {
        return new ParameterServer(ID);
    }

    extern "C" void destroy(ParameterServer* p) 
    {
        delete p;
    }
}