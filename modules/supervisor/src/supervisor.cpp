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
#ifndef SUPERVISOR_CPP
#define SUPERVISOR_CPP

#include "dls2/supervisor/supervisor.hpp"
#include "dls2/domains/domains.hpp"

namespace dls
{

	Supervisor::Supervisor(std::string ID_)
    : ID(ID_)
    , layersLink(ID, dls::domains::layers)
	{ }

    Supervisor::~Supervisor()
	{ }

    std::string Supervisor::getID()
    {
        return this->ID;
    }

    int Supervisor::getNumLayers()
    {
        // return layersLink.getParticipants().size()-1;
        auto layers = layersLink.getParticipants();

        return std::count_if(layers.begin(), layers.end(), [](std::string s) { return s.find("Layer") != std::string::npos; });
    }

    std::vector<std::string> Supervisor::getLayersNames()
    {
        return layersLink.getParticipants();
    }

    bool Supervisor::containsLayer(std::string name)
    {
        auto layers = layersLink.getParticipants();

        return (std::find(layers.begin(), layers.end(), name) != layers.end()); 
    }


}

#endif /* end of include guard: SUPERVISOR_CPP */