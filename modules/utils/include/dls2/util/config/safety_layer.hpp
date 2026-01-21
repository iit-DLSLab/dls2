#pragma once

#include "yaml-cpp/yaml.h"
#include <filesystem>
#include <unordered_map>

#include "dls2/util/time/duration_utils.hpp"

namespace dls
{
	struct SafetyLayerConfig{

		bool enable_process_died{false};
		bool enable_wrong_process_state{false};
		bool enable_wrong_process_frequency{false};
		bool enable_missing_input{false};
		bool enable_cpu_usage_too_high{false};
		bool enable_mem_usage_too_high{false};
		bool enable_inputs_not_synchronized{false};
		bool enable_wrong_input_frequency{false};
		bool enable_wrong_sequence_id{false};

		double spam_threshold{200};
        double max_exceeding_factor{0.5};
		double sync_threshold_ms{500};
		double realtime_tolerance_factor{0.3};
		size_t monitor_period_ms{100};
		size_t process_monitor_window_size {100};
		size_t resource_monitor_window_size {100};
		double cpu_threshold{100.0};
		double mem_threshold{100.0};
		std::unordered_map<std::string, double> nodes_specs;

		explicit SafetyLayerConfig(const std::string &config_file)
		{
			YAML::Node config;
			try
			{
				config = YAML::LoadFile(config_file);
			}
			catch (const YAML::BadFile &e)
			{
				std::cerr << "Error loading file: " << e.what() << "\n";
				return;
			}

			enable_process_died = config["checks"]["enable_process_died"].as<bool>();
			enable_wrong_process_state = config["checks"]["enable_wrong_process_state"].as<bool>();
			enable_wrong_process_frequency = config["checks"]["enable_wrong_process_frequency"].as<bool>();
			enable_missing_input = config["checks"]["enable_missing_input"].as<bool>();
			enable_cpu_usage_too_high = config["checks"]["enable_cpu_usage_too_high"].as<bool>();
			enable_mem_usage_too_high = config["checks"]["enable_mem_usage_too_high"].as<bool>();
			enable_inputs_not_synchronized = config["checks"]["enable_inputs_not_synchronized"].as<bool>();
			enable_wrong_input_frequency = config["checks"]["enable_wrong_input_frequency"].as<bool>();
			enable_wrong_sequence_id = config["checks"]["enable_wrong_sequence_id"].as<bool>();

            max_exceeding_factor = config["checks"]["max_exceeding_factor"].as<double>();
			sync_threshold_ms = config["checks"]["sync_threshold"].as<double>();
			realtime_tolerance_factor = config["checks"]["realtime_tolerance_factor"].as<double>();
			monitor_period_ms = config["checks"]["monitor_period"].as<size_t>();
            process_monitor_window_size = config["checks"]["process_monitor_window_size"].as<size_t>();
            resource_monitor_window_size = config["checks"]["resource_monitor_window_size"].as<size_t>();
            cpu_threshold = config["checks"]["cpu_threshold"].as<double>();
            mem_threshold = config["checks"]["mem_threshold"].as<double>();

			spam_threshold = config["events"]["spam_threshold"].as<double>();

			char * val;
			val = getenv("DLS_SCHEDULER_PATH");
			std::string sched_default_path = "/usr/include/dls2/schedulers";
			std::string sched_path = sched_default_path;
			if (val != NULL) {
				sched_path = val;
			}
			
			for (const auto& entry : std::filesystem::directory_iterator(sched_path)) {
				if (entry.is_regular_file() &&
					entry.path().extension() == ".yaml") {
						YAML::Node sched_config = YAML::LoadFile(entry.path());
						auto node_name = entry.path().stem();
						nodes_specs.insert_or_assign(node_name, 1.0 / (sched_config["period"].as<double>() / 1e3));
						nodes_specs.insert_or_assign(node_name, 1.0 / toSec(fromMs(sched_config["period"].as<double>())));
				}
			}
		}
	};
}