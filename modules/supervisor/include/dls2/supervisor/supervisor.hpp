
#ifndef SUPERVISOR_HPP
#define SUPERVISOR_HPP

#include <string>
#include <vector>
#include <list>

#include "dls2/command/command_manager.hpp"
#include "dls2/util/messaging/dds_participant.hpp"
#include "dls2/log/log.hpp"
#include "dls2/application/layer.hpp"

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

		virtual void monitor() override;

	private:

        dls::DDSParticipant layersLink;

		std::map<std::string, std::pair<std::string,bool>> app_states;
	};

	
} // end namespace dls

#endif /* end of include guard: SUPERVISOR_HPP */