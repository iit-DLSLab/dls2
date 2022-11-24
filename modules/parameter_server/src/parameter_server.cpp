#include "dls2/parameter_server/parameter_server.hpp"

#include <mutex>
#include <string>

namespace dls
{
	ParameterServer::ParameterServer() 
        : param_store_mutex()
        , param_store()
        , add_double(
			dls::topics::add_double,
			[this](ParamSetMsg msg) -> void
			{
				std::lock_guard<std::mutex> lock(this->param_store_mutex);
				this->param_store.insert({msg.key(), msg.value()});
			})
        , get_double(
			dls::topics::get_double,
			[this](StringMsg msg) -> DoubleMsg
			{
				std::lock_guard<std::mutex> lock(this->param_store_mutex);
				auto ret = this->param_store.find(msg.msg());

				DoubleMsg return_message;
				if (ret != this->param_store.end())
				{
					return_message.val() = ret->second;
				}
				else
				{
					return_message.val() = 0;
				}
				
				return return_message;
			}
		)
    {}
}