#pragma once

#include "yaml-cpp/yaml.h"

namespace dls
{
	struct SafetyLayerConfig{

		double spam_threshold{200};
        double max_exceeding_factor{0.5};
		double sync_threshold_ms{500};
		size_t monitor_period_ms{100};
		std::vector<std::pair<std::string, double>> topic_specs;

		explicit SafetyLayerConfig(const std::string &config_file)
		{
			YAML::Node config;
			try
			{
				config = YAML::LoadFile(config_file);
			}
			catch (const YAML::BadFile &e)
			{
				std::cerr << "Error loading periods file: " << e.what() << "\n";
				return;
			}

            max_exceeding_factor = config["checks"]["max_exceeding_factor"].as<double>();;
			sync_threshold_ms = config["checks"]["sync_threshold"].as<double>();
			spam_threshold = config["events"]["spam_threshold"].as<double>();
			monitor_period_ms = config["monitor"]["period"].as<size_t>();

			auto periods_config = config["periods"];
			for (auto it = periods_config.begin(); it != periods_config.end(); ++it)
			{
				std::string topic = it->first.as<std::string>();
				double period_ms = it->second.as<double>();
				topic_specs.push_back({topic, 1.0 / (period_ms / 1e3)});
			}
		}
	};
}