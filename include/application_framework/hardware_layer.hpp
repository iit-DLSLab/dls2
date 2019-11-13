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
#ifndef HARDWARE_LAYER_HPP_64INVL3T
#define HARDWARE_LAYER_HPP_64INVL3T

#include <vector>
#include <memory>
#include <mutex>

#include "application_framework/app_layer.hpp"
#include "sensors/sensor_base.hpp"

class HardwareLayer : public AppLayer
{
public:
	HardwareLayer();
	HardwareLayer(std::initializer_list<std::shared_ptr<SensorBase>>);

	Status run() override;
	Status shutdown() override;

	void addSensor(std::shared_ptr<SensorBase>);
	void addSensor(std::initializer_list<std::shared_ptr<SensorBase>>);

private:
	// BEGIN critical section
		std::vector<std::shared_ptr<SensorBase>> sensors;
		std::mutex sensors_mutex;
	// END critical section
};

#endif /* end of include guard: HARDWARE_LAYER_HPP_64INVL3T */
