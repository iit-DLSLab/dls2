
#ifndef PERIODIC_PLUGIN_BASE_TPP
#define PERIODIC_PLUGIN_BASE_TPP

#include "dls2/plugin_base/periodic_plugin_base.hpp"

using namespace dls;

template <typename MsgType, typename... constructor_args_types>
void PeriodicPluginBase::buildInput(dls::topicType &topic, WrapperBase* input, constructor_args_types... args)
{
    readers_.push_back(std::make_shared<SignalReader<MsgType>>(
															dds_participant_,
															topic,
															std::make_shared<MsgType>(args...)));
	inputs_.push_back(input);
}

template <typename MsgType, typename... constructor_args_types>
void PeriodicPluginBase::buildOutput(dls::topicType &topic, WrapperBase* output, constructor_args_types... args)
{
    writers_.push_back(std::make_shared<SignalWriter<MsgType>>(
															dds_participant_,
															topic,
															std::make_shared<MsgType>(args...)));
	outputs_.push_back(output);
}

#endif /* end of include guard: PERIODIC_PLUGIN_BASE_TPP */