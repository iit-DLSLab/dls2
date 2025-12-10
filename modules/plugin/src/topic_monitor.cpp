#include "dls2/plugin/topic_monitor.hpp"

namespace dls
{

    TopicMonitor::TopicMonitor(
		const std::map<std::string, size_t>& inputs_map,
		const std::string &periods_file)
    {
		inputs_map_ = inputs_map;
		savePeriodsFromFile(periods_file);

		for(const auto& [key, _] : expected_periods_){
			frequencies_moving_windows_[key] = std::make_unique<NumericalMovingWindow<double>>(FREQ_MOVING_WINDOW_DEFAULT_SIZE);
		}
	}

	void TopicMonitor::savePeriodsFromFile(const std::string &periods_file)
	{
		YAML::Node config;
		try
		{
			config = YAML::LoadFile(periods_file);
		}
		catch (const YAML::BadFile &e)
		{
			std::cerr << "Error loading periods file: " << e.what() << "\n";
			return;
		}

		sync_threshold_ms_ = config["checks"]["sync_threshold"].as<double>();
		auto periods_config = config["periods"];

		for (auto it = periods_config.begin(); it != periods_config.end(); ++it)
		{
			std::string key = it->first.as<std::string>();
			double value = it->second.as<double>();
			expected_periods_[key] = value;
		}
	}

	double TopicMonitor::getActualFrequency(const std::string& input_topic, const double& period_ms){

		double period_sec = period_ms / SEC_TO_MS;

		if (period_sec > 0.0)
		{
			const auto new_freq = 1.0 / period_sec;
			frequencies_moving_windows_[input_topic]->push(new_freq);
		}
		
		return frequencies_moving_windows_[input_topic]->mean();
	}

	std::map<std::string, double> TopicMonitor::getExpectedPeriods(){
		return expected_periods_;
	}

	std::map<std::string, double> TopicMonitor::computeFrequencies(const std::vector<double>& latest_periods_ms)
    {
        std::map<std::string, double> result;

        for (const auto &[input_topic, _] : inputs_map_)
		{
			const auto actual_frequency = this->getActualFrequency(input_topic, latest_periods_ms[inputs_map_.at(input_topic)]);
			result.emplace(input_topic, actual_frequency);
		}

        return result;
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

	bool TopicMonitor::isInputsMapEmpty(){
		return inputs_map_.empty();
	}

	void TopicMonitor::setInputMap(const std::map<std::string, size_t>& inputs_map){
		inputs_map_ = inputs_map;
	}

} // namespace dls