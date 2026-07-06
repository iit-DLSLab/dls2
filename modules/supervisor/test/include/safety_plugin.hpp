#ifndef SAFETY_PLUGIN_HPP
#define SAFETY_PLUGIN_HPP

#include "dls2/plugin/periodic_app_plugin.hpp"
// includes for I/O *************************
// -- readers/writers
#include "dls2/signal/reader.hpp"
#include "dls2/signal/writer.hpp"
// ******************************************

// plugin loaded at run-time
class SafetyPlugin : public dls::PeriodicAppPlugin
{
public:
	SafetyPlugin(const std::string& ID);

	virtual ~SafetyPlugin();

	void run(const std::chrono::system_clock::time_point &time) override;

	size_t cooldown_ms{ 800 };
};

#endif // end of include guard: SAFETY_PLUGIN_HPP
