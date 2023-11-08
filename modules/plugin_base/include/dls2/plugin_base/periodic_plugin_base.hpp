
#ifndef PLUGIN_BASE_HPP
#define PLUGIN_BASE_HPP

#include <string>

#include "dls2/application/periodic_app.hpp"
#include "dls2/msg_wrappers/signal_writer.hpp"
#include "dls2/msg_wrappers/signal_reader.hpp"

namespace dls
{
	class PeriodicPluginBase : public PeriodicApp
	{
	public:

		PeriodicPluginBase(const std::string& ID);
		
		virtual ~PeriodicPluginBase() = default;

		typedef PeriodicPluginBase *create_t(std::string);
		typedef void destroy_t(PeriodicPluginBase*);

		std::shared_ptr<dls::DDSParticipant> getParticipant();

		template <typename MessageType, typename... constructor_args_types>
		void buildInput(dls::topicType &topic, constructor_args_types... args);
		// template <typename MessageType, typename... constructor_args_types>
		// SignalWriter<MessageType> buildOutput<MessageType>(dls::topicType &topic, constructor_args_types... args);

		std::vector<void*> readers;
		std::vector<void*> writers;
		std::vector<void*> inputs;
		std::vector<void*> outputs;
		void readInputs();
		void publishOutputs();

	private:
		std::shared_ptr<dls::DDSParticipant> signalLink;
	};
} // end namespace dls

#include "dls2/plugin_base/periodic_plugin_base.tpp"

#endif /* PLUGIN_BASE_HPP */
