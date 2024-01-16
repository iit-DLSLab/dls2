
#ifndef PERIODIC_PLUGIN_BASE_TPP
#define PERIODIC_PLUGIN_BASE_TPP

#include "dls2/plugin_base/periodic_plugin_base.hpp"

namespace dls
{
	template <typename MsgWrapperType>
	void PeriodicPluginBase::buildInput(dls::topicType &topic, WrapperBase *input)
	{
		// Add data reader
		readers_.push_back(std::make_shared<SignalReader<MsgWrapperType>>(
			dds_participant_,
			topic,
			std::make_shared<MsgWrapperType>(static_cast<MsgWrapperType>(*input))));
		// Add pointer to input
		inputs_.push_back(input);
	}

	template <typename MsgWrapperType>
	void PeriodicPluginBase::buildOutput(dls::topicType &topic, WrapperBase *output)
	{
		// Add data writer
		writers_.push_back(std::make_shared<SignalWriter<MsgWrapperType>>(
			dds_participant_,
			topic,
			std::make_shared<MsgWrapperType>(static_cast<MsgWrapperType>(*output))));
		// Add pointer to output
		outputs_.push_back(output);
	}
}

#endif /* end of include guard: PERIODIC_PLUGIN_BASE_TPP */