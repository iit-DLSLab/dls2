
#ifndef PERIODIC_PLUGIN_BASE_HPP
#define PERIODIC_PLUGIN_BASE_HPP

#include "dls2/application/periodic_app.hpp"
#include "dls2/msg_wrappers/wrapper_base.hpp"
#include "dls2/msg_wrappers/signal_writer.hpp"
#include "dls2/msg_wrappers/signal_reader.hpp"

namespace dls
{
	class PeriodicPluginBase : public PeriodicApp
	{
	public:

		PeriodicPluginBase(const std::string& ID, const domainType& domain = domains::signals);
		
		virtual ~PeriodicPluginBase();

		typedef PeriodicPluginBase *create_t(std::string);
		typedef void destroy_t(PeriodicPluginBase*);

		template <typename MsgType, typename... constructor_args_types>
		void buildInput(dls::topicType &topic, WrapperBase* input, constructor_args_types... args);
		
		template <typename MsgType, typename... constructor_args_types>
		void buildOutput(dls::topicType &topic, WrapperBase* output, constructor_args_types... args);

		void read();
		
		void write();

		bool activate();
		bool deactivate();

	protected:
		bool active;

		std::shared_ptr<dls::DDSParticipant> dds_participant_;
	private:

		std::vector<std::shared_ptr<SignalReaderBase>> readers_;
		std::vector<std::shared_ptr<SignalWriterBase>> writers_;

		std::vector<WrapperBase*> inputs_;
		std::vector<WrapperBase*> outputs_;
	};
} // end namespace dls

#include "dls2/plugin_base/periodic_plugin_base.tpp"

#endif /* PERIODIC_PLUGIN_BASE_HPP */
