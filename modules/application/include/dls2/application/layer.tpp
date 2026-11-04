#ifndef LAYER_TPP_H7JRIVPM
#define LAYER_TPP_H7JRIVPM

#include "dls2/application/layer.hpp"
#include <vector>

namespace dls
{

template<class Map>
void Layer::checkAppData(const Map& app_data)
{
	checkAppDataImpl(app_data, nullptr);
}

template<class Map>
void Layer::checkAppData(const Map& app_data, std::mutex& mutex)
{
	checkAppDataImpl(app_data, &mutex);
}

template<class Map>
void Layer::checkAppDataImpl(const Map& app_data, std::mutex* mutex)
{
	using Ptr = typename Map::mapped_type;
    using Data = typename Ptr::element_type;
    static_assert(std::is_base_of_v<AppData, Data>, "must store AppData-derived");

	std::vector<std::string> stopped;
	std::unique_lock<std::mutex> lock;
	if (mutex) lock = std::unique_lock<std::mutex>(*mutex);
	for(const auto& [key, data] : app_data)
	{
		if(!data || !data->proc)
		{
			continue;
		}

		if(!data->proc->running())
		{
			if (this->safety_layer_config_->enable_process_died)
			{
				stopped.push_back(key);
			}
		}
	}
	if (lock.owns_lock()) lock.unlock();
	for (const auto& key : stopped)
		this->robust_event_notifier.notify(EventID::PROCESS_DIED, EventSeverity::ERROR,
			this->getID() + ": " + key + " is not running");
}

} // end namespace dls

#endif /* end of include guard: LAYER_TPP_H7JRIVPM */
