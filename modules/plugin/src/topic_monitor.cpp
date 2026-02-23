#include "dls2/plugin/topic_monitor.hpp"

namespace dls
{

    TopicMonitor::TopicMonitor(
		const std::string &ID,
		const double& max_exceeding_factor,
		const std::map<std::string, size_t>& inputs_map,
		const std::vector<InputInfo> &input_info,
		const std::shared_ptr<SafetyLayerConfig> &safety_layer_config)
    {
		sync_threshold_ms_ = safety_layer_config->sync_threshold_ms;
		nodes_specs_ = safety_layer_config->nodes_specs;
		plugin_id_ = ID;
		max_exceeding_factor_ = max_exceeding_factor;

		init(inputs_map, input_info);
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

	std::vector<bool> TopicMonitor::checkDesiredFrequency()
	{
		std::vector<bool> is_nominal;
		is_nominal.resize(input_topic_infos_.size());
		for (size_t i = 0; i < input_topic_infos_.size(); i++){
			const auto& topic_info = input_topic_infos_.at(i);

			if(enable_checks_.at(i) && topic_info.current_freq() > 0.0){
				// Desired frequency available, comparing it against the measured one 
				auto delta = abs(topic_info.desired_freq() - topic_info.current_freq());
				is_nominal.at(i) = delta < max_exceeding_factor_ * topic_info.desired_freq();
			}else{
				is_nominal.at(i) = true; // Not checking, desired frequency is not available
			}
		}
		return is_nominal;
	}

	std::vector<dls2_interface::msg::InputTopicInfo> TopicMonitor::getInputTopicInfo(const std::vector<InputInfo>& input_info)
	{
		if(inputsMapSize() > 0){
			for (auto &topic_info : input_topic_infos_)
			{				
				if(inputs_map_.find(topic_info.topic_name()) == inputs_map_.end()){
					continue;
				}

				// Latest measurements update
				const size_t input_info_idx = inputs_map_.at(topic_info.topic_name());
				const auto& input_info_item = input_info[input_info_idx];
				const auto latest_period_ms = input_info_item.latest_period_ms;
				const auto actual_frequency = this->getActualFrequency(topic_info.topic_name(), latest_period_ms);
								
				topic_info.current_freq() = actual_frequency;
				topic_info.missed_sequence_ids() = input_info_item.missed_sequence_ids;

				// Checking if desired frequency is up to date, if needed
				if(input_info_item.reader == nullptr || enable_checks_.at(input_info_idx)){
					continue;
				}
				auto topic_to_writer_map = input_info_item.reader->getTopicToWriter();

				const auto writer_it = topic_to_writer_map.find(topic_info.topic_name()); 
				if(writer_it == topic_to_writer_map.end()){
					continue;
				}

				auto node_it = nodes_specs_.find(writer_it->second);
				if(node_it == nodes_specs_.end()){
					enable_checks_.at(input_info_idx) = false;
				}else{
					auto desired_freq = node_it->second;
					topic_info.desired_freq() = desired_freq;
					enable_checks_.at(input_info_idx) = true;
				}
			}
		}

		return input_topic_infos_;

	}

	bool TopicMonitor::areTopicsSync(const std::vector<InputInfo>& input_info){
		if(input_info.size() > 1){

			// Choose an input reference (first valid reader)
			size_t ref_idx = 0;
			while(ref_idx < input_info.size() && !input_info[ref_idx].reader->is_receiving_data()){
				ref_idx++;
			}
			if(ref_idx >= input_info.size()){
				// No valid readers available
				return true;
			}
			const auto& input_reference = input_info[ref_idx];

			// Evaluate remaining valid readers comparing them to input reference
			for(size_t i = 0; i < input_info.size(); i++){
				if(i == ref_idx || !input_info[i].reader->is_receiving_data()){
					continue;
				}
				auto delta_time = abs(toMs<double>(input_reference.latest_timestamp - input_info[i].latest_timestamp));
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

	void TopicMonitor::init(const std::map<std::string, size_t>& inputs_map, 
								   const std::vector<InputInfo> & input_info)
	{
		inputs_map_ = inputs_map;

		input_topic_infos_.clear();
		enable_checks_.clear();
		frequencies_moving_windows_.clear();

		for (auto &topic_info : input_info){

			auto topic = topic_info.topic_name;

			dls2_interface::msg::InputTopicInfo info;
			info.topic_name() = topic;
			info.desired_freq() = 0.0;
			info.current_freq() = 0.0;
			
			if(inputs_map.find(topic) != inputs_map.end()){
				input_topic_infos_.push_back(info);
				enable_checks_.push_back(false);
				frequencies_moving_windows_[topic] = std::make_unique<NumericalMovingWindow<double>>(FREQ_MOVING_WINDOW_DEFAULT_SIZE);
			}
		}
	}

} // namespace dls