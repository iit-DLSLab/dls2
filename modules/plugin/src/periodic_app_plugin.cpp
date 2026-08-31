#include "dls2/plugin/periodic_app_plugin.hpp"

namespace dls
{
	PeriodicAppPlugin::PeriodicAppPlugin(const std::string &ID, const domainType &domain)
		: PeriodicApp(ID), Plugin(ID, domain)
	{
		topic_monitor_ = std::make_unique<TopicMonitor>(ID, this->safety_layer_config_->max_exceeding_factor, 
			this->inputs_map, this->input_info_, this->safety_layer_config_);
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

		std::vector<InputInfo> input_info{};
		{
			std::lock_guard<std::mutex> lock(input_info_mutex_);
			input_info = this->input_info_;
		}

		if(topic_monitor_->inputsMapSize() != inputs_map.size()){
			topic_monitor_->init(this->inputs_map, input_info);
		}

		if(this->safety_layer_config_->enable_wrong_sequence_id){
			for(size_t i = 0; i < input_info.size(); ++i){
				if(input_info.at(i).missed_sequence_ids != 0 
					&& input_info.at(i).topic_name != this->safety_layer_config_->process_status_topic){
					this->robust_event_notifier.notify(
						EventID::WRONG_SEQUENCE_ID,
						EventSeverity::WARNING,
						this->getID() + " app detected wrong sequence id from topic " + input_info.at(i).topic_name 
							+ " (missed " + std::to_string(input_info.at(i).missed_sequence_ids) + ")\n"
					);
				}
			}
		}

		if(this->safety_layer_config_->enable_missing_input && missing_inputs.str()!=""){
			this->robust_event_notifier.notify(
						EventID::MISSING_INPUT,
						EventSeverity::WARNING,
						this->getID() + " app detected missing input topic " + missing_inputs.str() + "\n"
					);
		}

		status_msg.input_topic_info() = topic_monitor_->getInputTopicInfo(input_info); 

		if(this->safety_layer_config_->enable_wrong_input_frequency){
			auto nominalFrequency = topic_monitor_->checkDesiredFrequency();

			for(size_t i = 0; i < nominalFrequency.size(); ++i){
				if(!nominalFrequency.at(i)){
					const auto& info = status_msg.input_topic_info().at(i);
					this->robust_event_notifier.notify(
						EventID::WRONG_INPUT_FREQUENCY,
						EventSeverity::WARNING,
						this->getID() + " app detected unexpected input frequency (" + std::to_string(info.current_freq()) 
							+ " Hz vs " + std::to_string(info.desired_freq()) +" Hz) on topic " + info.topic_name() + "\n"
					);
				}
			}
		}
		
		bool are_inputs_sync = topic_monitor_->areTopicsSync(input_info);

		if(this->safety_layer_config_->enable_inputs_not_synchronized && !are_inputs_sync){
			this->robust_event_notifier.notify(
				EventID::INPUTS_NOT_SYNCHRONIZED,
				EventSeverity::WARNING,
				this->getID() + " app detected not synchronized input topics\n"
			);
		}

		status_msg.inputs_synchronized() = static_cast<int32_t>(are_inputs_sync); 
		status_msg.status_string() = ""; // TODO: fill in
	}
}
