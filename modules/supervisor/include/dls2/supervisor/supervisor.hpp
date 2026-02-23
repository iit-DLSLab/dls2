
#ifndef SUPERVISOR_HPP
#define SUPERVISOR_HPP

#include <string>
#include <vector>
#include <list>

#include "dls2/command/command_manager.hpp"
#include "dls2/util/messaging/dds_participant.hpp"
#include "dls2/log/log.hpp"
#include "dls2/application/layer.hpp"
#include "dls2/state_machine/state_machine_watcher.hpp"
#include "dls2/application/app_data.hpp"
#include "dls2/util/system_resource_monitor.hpp"

namespace dls
{
	class Supervisor: public Layer
	{
	public:
		Supervisor(std::string ID, const std::string& robot_name);
		~Supervisor();

		int getNumLayers();
		std::vector<std::string> getLayersNames();
		bool containsLayer(std::string name);

		bool loadPeriodicAppPlugin(const std::string& ID);
		bool loadAppPlugin(const std::string& ID);

		virtual void monitor() override;

		void checkHardware();

	private:
        dls::DDSParticipant ddspart_layers;
        state_machine::StateMachineWatcher state_machine_watcher;

		std::unique_ptr<dls::SystemResourceMonitor> sys_monitor;

		std::map<std::string, std::shared_ptr<AppData>> plugins;

		const std::string robot_name;
	};

	
} // end namespace dls

#endif /* end of include guard: SUPERVISOR_HPP */