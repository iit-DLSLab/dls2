
#ifndef PERIODIC_PLUGIN_BASE_TPP
#define PERIODIC_PLUGIN_BASE_TPP

#include "dls2/plugin_base/periodic_plugin_base.hpp"

namespace dls
{
	// template <typename MsgWrapperType>
	// void PeriodicPluginBase::buildInput(const dls::topicType &topic, WrapperBase *input, const std::function<void()> &auxiliary_callback, bool required_on_activation)
	// {
	// 	// Add data reader
	// 	readers_.push_back(std::make_shared<SignalReader<MsgWrapperType>>(
	// 		dds_participant_,
	// 		topic,
	// 		std::make_shared<MsgWrapperType>(static_cast<MsgWrapperType &>(*input)),
	// 		auxiliary_callback));
	// 	// Add pointer to input
	// 	inputs_.push_back(input);

	// 	// Add activation requirement info
	// 	are_inputs_required_on_activation.push_back(required_on_activation);
	// }

	// template <typename MsgWrapperType>
	// void PeriodicPluginBase::buildInput(const std::string& name, const dls::topicType &topic, WrapperBase *input, const std::function<void()> &auxiliary_callback, bool required_on_activation)
	// {
	// 	// Add data reader
	// 	readers_.push_back(std::make_shared<SignalReader<MsgWrapperType>>(
	// 		dds_participant_,
	// 		topic,
	// 		std::make_shared<MsgWrapperType>(static_cast<MsgWrapperType &>(*input)),
	// 		auxiliary_callback));
	// 	// Add pointer to input
	// 	inputs_.push_back(input);

	// 	// Add output to the map
	// 	readers_map_[name] = std::make_pair(readers_.back(), input);

	// 	// Add activation requirement info
	// 	are_inputs_required_on_activation.push_back(required_on_activation);
	// }

	// template <typename MsgWrapperType>
	// void PeriodicPluginBase::buildOutput(const dls::topicType &topic, WrapperBase *output)
	// {
	// 	// Add data writer
	// 	writers_.push_back(std::make_shared<SignalWriter<MsgWrapperType>>(
	// 		dds_participant_,
	// 		topic,
	// 		std::make_shared<MsgWrapperType>(static_cast<MsgWrapperType &>(*output))));
	// 	// Add pointer to output
	// 	outputs_.push_back(output);
	// }

	// template <typename MsgWrapperType>
	// void PeriodicPluginBase::buildOutput(const std::string& name, const dls::topicType &topic, WrapperBase *output)
	// {
	// 	// Add data writer
	// 	writers_.push_back(std::make_shared<SignalWriter<MsgWrapperType>>(
	// 		dds_participant_,
	// 		topic,
	// 		std::make_shared<MsgWrapperType>(static_cast<MsgWrapperType &>(*output))));
	// 	// Add pointer to output
	// 	outputs_.push_back(output);

	// 	// Add output to the map
	// 	writers_map_[name] = std::make_pair(writers_.back(), output);
	// }
}

#endif /* end of include guard: PERIODIC_PLUGIN_BASE_TPP */