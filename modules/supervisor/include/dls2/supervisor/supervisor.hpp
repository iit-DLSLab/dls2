
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

namespace dls
{
	class Supervisor: public Layer
	{
	public:
		Supervisor(std::string ID);
		~Supervisor();

		int getNumLayers();
		std::vector<std::string> getLayersNames();
		bool containsLayer(std::string name);

		bool loadPeriodicAppPlugin(const std::string& ID);
		bool loadAppPlugin(const std::string& ID);

		virtual void monitor() override;

	private:
        dls::DDSParticipant ddspart_layers;
        state_machine::StateMachineWatcher state_machine_watcher;

		std::map<std::string, std::shared_ptr<AppData>> plugins;
	};

	
} // end namespace dls

#endif /* end of include guard: SUPERVISOR_HPP */