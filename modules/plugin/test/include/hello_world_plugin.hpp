#ifndef hello_world_plugin_HPP
#define hello_world_plugin_HPP

#include "dls2/plugin/periodic_app_plugin.hpp"
// includes for I/O *************************
// -- readers/writers
#include "dls2/signal/reader.hpp"
#include "dls2/signal/writer.hpp"
// ******************************************

// plugin loaded at run-time
class HelloWorldPlugin : public dls::PeriodicAppPlugin
{
public:
	HelloWorldPlugin(const std::string& ID);

	virtual ~HelloWorldPlugin();

	void run(const std::chrono::system_clock::time_point &time) override;

	// console commands
	bool setJointTorque();

	// I/O definition
	dls::ReaderPtr<dls2_interface::msg::BlindState> reader_bs;
	dls::WriterPtr<dls2_interface::msg::ControlSignal> writer_cs;
};

#endif // end of include guard: hello_world_plugin_HPP
