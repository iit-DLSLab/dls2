#include "dls2/parameter_server/parameter_server.hpp"

#include <mutex>
#include <string>

namespace dls
{
	ParameterServer::ParameterServer(std::string& ID) 
		: Service(
			ID,
			dls::topics::param_server,
			[this](ParamServerMsg msg) -> DoubleMsg
			{
				std::lock_guard<std::mutex> lock(this->param_store_mutex);
				auto param = this->param_store.find(msg.key());

				DoubleMsg return_msg;

				// if it is a reading request and the parameter does not exists return 0
				if(param == this->param_store.end() && !msg.rw())
				{
					return_msg.value() = 0;
					return return_msg;
				}

				// update or insert the parameter if it is a writing request
				if(msg.rw())
				{
					param = param_store.emplace(msg.key(), msg.value()).first;
				}		

				// return the current value of the parameter
				return_msg.value() = param->second;
				return return_msg;
			})
        , param_store_mutex()
        , param_store()
    {
		scout << "SERVICE " + ID + " IS RUNNING" << std::endl;
	}

	ParameterServer::~ParameterServer() 
	{
		scout << "SERVICE " + this->getID() + " IS OFF" << std::endl;
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