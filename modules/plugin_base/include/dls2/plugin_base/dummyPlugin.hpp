
#ifndef PLUGIN_BASE_HPP
#define PLUGIN_BASE_HPP

#include <string>

#include "dls2/application/periodic_app.hpp"

namespace dls
{
	class PluginBase : public PeriodicApp
	{
	public:

		PluginBase(const std::string& ID);
		
		virtual ~PluginBase() = default;

		typedef PluginBase *create_t(std::string);
		typedef void destroy_t(PluginBase*);

		std::shared_ptr<dls::DDSParticipant> getParticipant();

	private:
		std::shared_ptr<dls::DDSParticipant> signalLink;
		//inputs
		//outputs	
	};
} // end namespace dls

	// template <typename T>
	// SignalReader<T> buildInput<T>(topic, args...);
	// template <typename T>
	// SignalWriter<T> buildOutput<MessageType>(topic, args...);

	// void readInputs();
	// void publishOutputs();

	// input<message, topic>(args);


	// void readInputs()
	// {
	// 	inputs.blind_state = inputs.readers.blind_state.getData();
	// }


#endif /* PLUGIN_BASE_HPP */