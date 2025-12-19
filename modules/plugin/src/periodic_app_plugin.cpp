#include "dls2/plugin/periodic_app_plugin.hpp"

namespace dls
{
	PeriodicAppPlugin::PeriodicAppPlugin(const std::string &ID, const domainType &domain, const std::string &periods_file)
		: PeriodicApp(ID), Plugin(ID, domain)
	{
		topic_monitor_ = std::make_unique<TopicMonitor>(this->inputs_map, periods_file);
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
		PeriodicApp::childMonitor();

		if(topic_monitor_->isInputsMapEmpty()){
			topic_monitor_->setInputMap(this->inputs_map);
		}

		std::vector<InputInfo> input_info{};
		{
			std::lock_guard<std::mutex> lock(input_info_mutex_);
			input_info = this->input_info_;
		}

		for(size_t i = 0; i < input_info.size(); ++i){
			if(!input_info.at(i).sequence_id_sane){
				this->robust_event_notifier.notify(
					EventID::WRONG_SEQUENCE_ID,
					EventSeverity::WARNING,
					this->getID() + " app detected wrong sequence id from topic " + input_info.at(i).topic_name + "\n"
				);
			}
		}

		status_msg.inputs_current_freq() = topic_monitor_->computeFrequencies(input_info);
		status_msg.inputs_desired_freq() = topic_monitor_->getExpectedPeriods();

		bool are_inputs_sync = topic_monitor_->areTopicsSync(input_info);

		status_msg.inputs_synchronized() = static_cast<int32_t>(are_inputs_sync); 
		status_msg.status_string() = ""; // TODO: fill in
	}
}
