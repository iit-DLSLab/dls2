#ifndef LAYER_TPP_H7JRIVPM
#define LAYER_TPP_H7JRIVPM

#include "dls2/application/layer.hpp"

namespace dls
{

template<class Map>
void Layer::checkAppData(const Map& app_data)
{
	using Ptr = typename Map::mapped_type;
    using Data = typename Ptr::element_type;
    static_assert(std::is_base_of_v<AppData, Data>, "must store AppData-derived");

	for(const auto& [key, data] : app_data)
	{
		if(!data->proc->running())
		{
			if (this->safety_layer_config_->enable_process_died)
			{
				this->robust_event_notifier.notify(
					EventID::PROCESS_DIED,
					EventSeverity::ERROR,
					this->getID() + ": " + key + " is not running"
				);
			}
		}
	}
}

} // end namespace dls

#endif /* end of include guard: LAYER_TPP_H7JRIVPM */
