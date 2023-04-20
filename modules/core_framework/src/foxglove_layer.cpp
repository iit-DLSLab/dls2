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
#include "dls2/core_framework/foxglove_layer.hpp"

using namespace dls;

FoxgloveLayer::FoxgloveLayer(std::string ID) :
	Layer(ID)
{ 
	scout_sys << "Foxglove layer loaded" << std::endl;
}

FoxgloveLayer::~FoxgloveLayer()
{ }

AppStatus FoxgloveLayer::run()
{
	while(!this->should_quit)
	{
		std::this_thread::sleep_for(std::chrono::duration<double, std::milli>(300));
	}
	return getStatus();
}

AppStatus FoxgloveLayer::stop()
{
	this->should_quit = true;

	setStatus(AppStatus::STOPPED);
	return getStatus();
}