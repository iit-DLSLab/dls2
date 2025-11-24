#include "dls2/plugin/periodic_app_plugin.hpp"

namespace dls
{
	PeriodicAppPlugin::PeriodicAppPlugin(const std::string &ID, const domainType &domain, const std::string &periods_file)
		: PeriodicApp(ID), Plugin(ID, domain)
	{
		savePeriodsFromFile(periods_file);
	}

	PeriodicAppPlugin::~PeriodicAppPlugin()
	{}

	bool PeriodicAppPlugin::checkActivation()
	{
		return basicActivationChecks();
	}

	bool PeriodicAppPlugin::basicActivationChecks(){
		bool res = areInputsReceivingData() && areOutputsUnique();
		if(missing_inputs.str()!=""){
			activation_message << "Missing inputs: " << missing_inputs.str() << "\n";;
		}
		if(common_outputs.str()!=""){
			activation_message << "There is at list another writer publishing on the topics: " << common_outputs.str() << "\n";
		}
		return res;
	}

	void PeriodicAppPlugin::childMonitor()
	{
		if (this->monitor_period_ms > this->dt)
		{
			this->monitor_period_ms = static_cast<size_t>(this->dt);
		}
		bool are_inputs_sync = true;
		status_msg.current_frequency() = current_frequency;
		status_msg.desired_frequency() = getDesiredFrequency();
		status_msg.realtime() = static_cast<uint8_t>(sm.state->realtime);
		status_msg.inputs_current_freq() = getInputsFrequency(are_inputs_sync);
		status_msg.inputs_desired_freq() = getInputsDesiredFrequency();
		status_msg.inputs_synchronized() = static_cast<int32_t>(are_inputs_sync);

		showProcessInfo();			  // TODO: working? here or in app.cpp?
		status_msg.cpu_usage() = 0.0; // TODO: calculate

		std::cout << "sendind status... " << static_cast<int32_t>(are_inputs_sync) << "\n";
		status_notifier.sendMessage(&status_msg);
	}

	void PeriodicAppPlugin::savePeriodsFromFile(const std::string &periods_file)
	{
		YAML::Node config = YAML::LoadFile(periods_file);

		for (auto it = config.begin(); it != config.end(); ++it)
		{
			std::string key = it->first.as<std::string>();
			double value = it->second.as<double>();
			periods_map[key] = value;
			std::cout << "reading period: " << key << ", " << value << "\n";
		}
	}

	std::map<std::string, double> PeriodicAppPlugin::getInputsFrequency(bool &are_inputs_sync)
	{
		std::map<std::string, double> inputs_freq;
		for (const auto &[input_topic, _] : this->inputs_map)
		{
			double current_frequency = 0.0;
			auto is_input_sync = Time::checkFrequency(periods_map[input_topic], loop_time_prec_map[input_topic], current_frequency);
			if (!is_input_sync && are_inputs_sync)
			{
				are_inputs_sync = false;
			}
			inputs_freq.emplace(input_topic, current_frequency);
		}
		return inputs_freq;
	}

	std::map<std::string, double> PeriodicAppPlugin::getInputsDesiredFrequency()
	{
		std::map<std::string, double> inputs_desired_freq;
		for (const auto &[input_topic, _] : this->inputs_map)
		{
			inputs_desired_freq.emplace(input_topic, periods_map[input_topic]);
		}
		return inputs_desired_freq;
	}

}
