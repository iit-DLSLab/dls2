#pragma once

#include <mutex>
#include <chrono>
#include <string>
#include <unordered_map>
#include <optional>
#include "yaml-cpp/yaml.h"
#include <iostream>

#include "dls2/util/time/duration_utils.hpp"
#include "dls2/plugin/numerical_moving_window.tpp"
#include "dls2/plugin/plugin.hpp"

namespace dls
{

class TopicMonitor
{

	static constexpr size_t FREQ_MOVING_WINDOW_DEFAULT_SIZE = 1000;
	static constexpr double SEC_TO_MS = 1e3;

public:

    explicit TopicMonitor(const std::map<std::string, size_t>& inputs_map, const std::string &periods_file);

	void savePeriodsFromFile(const std::string &periods_file);

	[[nodiscard]] double getActualFrequency(const std::string& input_topic, const double& period_ms);

	[[nodiscard]] std::map<std::string, double> getExpectedPeriods();

	[[nodiscard]] std::map<std::string, double> computeFrequencies(const std::vector<double>& latest_periods_ms);
	[[nodiscard]] std::map<std::string, double> computeFrequencies(const std::vector<InputInfo>& input_info);

	[[nodiscard]] bool areTopicsSync(const std::vector<std::chrono::steady_clock::time_point>& latest_timestamp);
	[[nodiscard]] bool areTopicsSync(const std::vector<InputInfo>& input_info);


	[[nodiscard]] bool isInputsMapEmpty();

	void setInputMap(const std::map<std::string, size_t>& inputs_map);

private:

	// topic name -> expected period mapping
 	std::map<std::string, double> expected_periods_;

	// topic name -> frequency moving window mapping
    std::map<std::string, std::unique_ptr<NumericalMovingWindow<double>>> frequencies_moving_windows_;

	// topic name -> index in inputs vector mapping
	std::map<std::string, size_t> inputs_map_;

	// topic sync threshold
	double sync_threshold_ms_{ 1000 };
};


} // end namespace dls
