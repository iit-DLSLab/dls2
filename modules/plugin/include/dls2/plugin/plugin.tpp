#ifndef PLUGIN_TPP
#define PLUGIN_TPP

#include "dls2/plugin/plugin.hpp"

namespace dls
{
	template <typename MsgType>
	dls::ReaderPtr<MsgType> Plugin::buildInput(const dls::topicType &topic, const std::function<void()> &auxiliary_callback, bool required_on_activation)
	{
		InputInfo input_info{};
		ReaderPtr<MsgType> reader = std::make_shared<dls::Reader<MsgType>>(dds_participant_,topic,auxiliary_callback);
		input_info.reader = reader;
		input_info.latest_period_ms = 0.0;
		input_info.latest_timestamp = std::chrono::steady_clock::now();
		input_info.latest_sequence_id = 0;
		input_info.sequence_id_sane = true;
		input_info.are_inputs_required_on_activation = required_on_activation;
		input_info.topic_name = topic.first;

		std::lock_guard<std::mutex> lock(input_info_mutex_);

		input_info_.push_back(input_info);

		// TODO: Add number if topic is not unique
		inputs_map[topic.first] = input_info_.size() - 1; // Store the index of the input in the inputs vector
	
		return reader;
	}

	template <typename MsgType>
	dls::WriterPtr<MsgType> Plugin::buildOutput(const dls::topicType &topic)
	{
		OutputInfo output_info{};
		WriterPtr<MsgType> writer = std::make_shared<dls::Writer<MsgType>>(dds_participant_,topic);
		output_info.writer = writer;
		output_info.topic_name = topic.first;
		
		std::lock_guard<std::mutex> lock(output_info_mutex_);

		output_info_.push_back(output_info);

		// TODO: Add number if topic is not unique
		outputs_map[topic.first] = output_info_.size() - 1; // Store the index of the output in the outputs vector

		return writer;
	}
}

#endif /* end of include guard: PLUGIN_TPP */
