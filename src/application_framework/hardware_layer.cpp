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
********************************************************************************
* Author:            Hendrik de Bruin                                          *
* Maintainer:        Hendrik de Bruin                                          *
* author email:      hendrik.debruin@iit.it                                    *
*******************************************************************************/
#include "application_framework/hardware_layer.hpp"

// =============================================================================
// Constructors
// =============================================================================
dls::HardwareLayer::HardwareLayer() :
	sensors(),
	sensors_mutex()
{ }

dls::HardwareLayer::HardwareLayer
(
	std::initializer_list<std::shared_ptr<SensorBase>> _sensors
) :
	sensors(_sensors),
	sensors_mutex()
{ }

// =============================================================================
// Member Functions
// =============================================================================
dls::HardwareLayer::Status dls::HardwareLayer::run()
{
	return getStatus();
}

dls::HardwareLayer::Status dls::HardwareLayer::shutdown()
{
	return getStatus();
}

void dls::HardwareLayer::addSensor(std::shared_ptr<SensorBase> pSensor)
{
	std::lock_guard<std::mutex> lock(this->sensors_mutex);
	this->sensors.push_back(pSensor);
}

void dls::HardwareLayer::addSensor
(
	std::initializer_list<std::shared_ptr<SensorBase>> in_sensors
)
{
	std::lock_guard<std::mutex> lock(this->sensors_mutex);
	this->sensors.insert(this->sensors.end(), in_sensors.begin(), in_sensors.end());
}
