#ifndef PLUGIN_TPP
#define PLUGIN_TPP

#include "dls2/plugin/plugin.hpp"

namespace dls
{
	template <typename MsgType>
	dls::ReaderPtr<MsgType> Plugin::buildInput(const dls::topicType &topic, const std::function<void()> &auxiliary_callback, bool required_on_activation)
	{
		// Add data reader
		ReaderPtr<MsgType> reader = std::make_shared<dls::Reader<MsgType>>(dds_participant_,topic,auxiliary_callback);
		inputs.push_back(reader);
		inputs_latest_periods_ms.push_back(0.0);
		// TODO: Add number if topic is not unique
		inputs_map[topic.first] = inputs.size() - 1; // Store the index of the input in the inputs vector
	
		// Add activation requirement info
		are_inputs_required_on_activation.push_back(required_on_activation);

		return reader;
	}

	template <typename MsgType>
	dls::WriterPtr<MsgType> Plugin::buildOutput(const dls::topicType &topic)
	{
		// Add data writer
		WriterPtr<MsgType> writer = std::make_shared<dls::Writer<MsgType>>(dds_participant_,topic);
		outputs.push_back(writer);
		// TODO: Add number if topic is not unique
		outputs_map[topic.first] = outputs.size() - 1; // Store the index of the output in the outputs vector

		return writer;
	}
}

#endif /* end of include guard: PLUGIN_TPP */
