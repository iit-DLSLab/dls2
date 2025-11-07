#ifndef CUSTOM_PLUGIN_HPP
#define CUSTOM_PLUGIN_HPP

#include "dls2/plugin/periodic_app_plugin.hpp"
// includes for I/O *************************
// -- readers/writers
#include "dls2/signal/reader.hpp"
#include "dls2/signal/writer.hpp"
// -- messages
#include "dls_messages/dds/blind_state.hpp"
#include "dls_messages/dds/control_signal.hpp"
// ******************************************

// plugin loaded at run-time
class CustomPlugin : public dls::PeriodicAppPlugin
{
public:
	CustomPlugin(const std::string& ID);
	
	virtual ~CustomPlugin();

	void run(const std::chrono::system_clock::time_point &time) override;
	
	// console commands
	bool setJointTorque();

	// I/O definition
	dls::ReaderPtr<dls2_interface::msg::BlindState> reader_bs;
	dls::WriterPtr<dls2_interface::msg::ControlSignal> writer_cs;
};

#endif // end of include guard: CUSTOM_PLUGIN_HPP