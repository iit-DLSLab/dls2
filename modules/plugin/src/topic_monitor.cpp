#include "dls2/plugin/topic_monitor.hpp"

namespace dls
{

    TopicMonitor::TopicMonitor(
		const std::map<std::string, size_t>& inputs_map,
		const std::shared_ptr<SafetyLayerConfig> &safety_layer_config)
    {
		sync_threshold_ms_ = safety_layer_config->sync_threshold_ms;

		inputs_map_ = inputs_map;

		for(const auto& [topic, desired_freq] : safety_layer_config->topic_specs){
			if(inputs_map.find(topic) != inputs_map.end()){

				dls2_interface::msg::InputTopicInfo info;
				info.topic_name() = topic;
				info.desired_freq() = desired_freq;
				info.current_freq() = desired_freq;
				input_topic_infos_.push_back(info);
				
				frequencies_moving_windows_[topic] = std::make_unique<NumericalMovingWindow<double>>(FREQ_MOVING_WINDOW_DEFAULT_SIZE);
			}
		}
	}

	double TopicMonitor::getActualFrequency(const std::string& input_topic, const double& period_ms){

		double period_sec = toSec(fromMs(period_ms));

		if (period_sec > 0.0)
		{
			const auto new_freq = 1.0 / period_sec;
			frequencies_moving_windows_[input_topic]->push(new_freq);
		}
		
		return frequencies_moving_windows_[input_topic]->mean();
	}

	std::vector<dls2_interface::msg::InputTopicInfo> TopicMonitor::getInputTopicInfo(const std::vector<InputInfo>& input_info){

		if(inputsMapSize() < 1){
			for (auto &topic_info : input_topic_infos_)
			{				
				const auto latest_period_ms = input_info[inputs_map_.at(topic_info.topic_name())].latest_period_ms;
				const auto actual_frequency = this->getActualFrequency(topic_info.topic_name(), 
																	latest_period_ms);
				topic_info.current_freq() = actual_frequency;
			}
		}
		return input_topic_infos_;

	}

	bool TopicMonitor::areTopicsSync(const std::vector<std::chrono::steady_clock::time_point>& latest_timestamp){
		if(latest_timestamp.size() > 1){
			for(size_t i = 1; i < latest_timestamp.size(); i++){
				auto delta_time = abs(toMs<double>(latest_timestamp[0] - latest_timestamp[i]));
				if(delta_time > sync_threshold_ms_){
					return false;
				}
			}
		}
		return true;
	}

	bool TopicMonitor::areTopicsSync(const std::vector<InputInfo>& input_info){
		if(input_info.size() > 1){
			for(size_t i = 1; i < input_info.size(); i++){
				auto delta_time = abs(toMs<double>(input_info[0].latest_timestamp - input_info[i].latest_timestamp));
				if(delta_time > sync_threshold_ms_){
					return false;
				}
			}
		}
		return true;
	}

	size_t TopicMonitor::inputsMapSize(){
		return inputs_map_.size();
	}

	void TopicMonitor::setInputMap(const std::map<std::string, size_t>& inputs_map){
		inputs_map_ = inputs_map;
	}

} // namespace dls