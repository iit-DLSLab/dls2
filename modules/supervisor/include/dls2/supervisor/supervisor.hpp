/*******************************************************************************
*                                                       ,----,                 *
*                                                     .'   .' \                *
*                                                   ,----,'    |               *
*               ________  ___       ________        |    :  .  ;               *
*              |\   ___ \|\  \     |\   ____\       ;    |.'  /                *
*              \ \  \_|\ \ \  \    \ \  \___|_      `----'/  ;                 *
*               \ \  \ \\ \ \  \    \ \_____  \       /  ;  /                  *
*                \ \  \_\\ \ \  \____\|____|\  \     ;  /  /-,                 *
*                 \ \_______\ \_______\____\_\  \   /  /  /.`|                 *
*                  \|_______|\|_______|\_________\./__;      :                 *
*                                     \|_________||   :    .'                  *
*                                                 ;   | .'                     *
*                                                 `---'                        *
*******************************************************************************/
#ifndef SUPERVISOR_HPP
#define SUPERVISOR_HPP

#include <string>
#include <vector>
#include <list>

#include "dls2/command/command_manager.hpp"
#include "dls2/util/messaging/dds_participant.hpp"

namespace dls
{
	class Supervisor
	{
	public:
		Supervisor(std::string ID);
		~Supervisor();

        std::string getID();

		int getNumLayers();
		std::vector<std::string> getLayersNames();
		bool containsLayer(std::string name);

	private:

        std::string ID;
		
        dls::DDSParticipant layersLink;

	};

	
} // end namespace dls

#endif /* end of include guard: SUPERVISOR_HPP */