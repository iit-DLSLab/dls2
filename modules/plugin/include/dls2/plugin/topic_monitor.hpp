#pragma once

#include <mutex>
#include <chrono>
#include <string>
#include <unordered_map>
#include <optional>
#include <iostream>

#include "dls2/util/time/duration_utils.hpp"
#include "dls2/plugin/numerical_moving_window.hpp"
#include "dls2/plugin/plugin.hpp"
#include <dls2/application/config/safety_layer.hpp>

namespace dls
{

class TopicMonitor
{
	static constexpr size_t FREQ_MOVING_WINDOW_DEFAULT_SIZE = 1000;

public:

    explicit TopicMonitor(const std::map<std::string, size_t>& inputs_map, const std::shared_ptr<SafetyLayerConfig> &safety_layer_config);

	[[nodiscard]] double getActualFrequency(const std::string& input_topic, const double& period_ms);

	[[nodiscard]] std::vector<dls2_interface::msg::InputTopicInfo> getInputTopicInfo(const std::vector<InputInfo>& input_info);

	[[nodiscard]] bool areTopicsSync(const std::vector<std::chrono::steady_clock::time_point>& latest_timestamp);
	[[nodiscard]] bool areTopicsSync(const std::vector<InputInfo>& input_info);

	[[nodiscard]] size_t inputsMapSize();
	void setInputMap(const std::map<std::string, size_t>& inputs_map);

private:

	// topic name -> frequency moving window mapping
    std::map<std::string, std::unique_ptr<NumericalMovingWindow<double>>> frequencies_moving_windows_;

	// topic name -> index in inputs vector mapping
	std::map<std::string, size_t> inputs_map_;

	std::vector<dls2_interface::msg::InputTopicInfo> input_topic_infos_;

	// topic sync threshold
	double sync_threshold_ms_{ 1000 };
};


} // end namespace dls
