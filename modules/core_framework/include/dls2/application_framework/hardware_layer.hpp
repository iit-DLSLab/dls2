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
#ifndef HARDWARE_LAYER_HPP_64INVL3T
#define HARDWARE_LAYER_HPP_64INVL3T

#include "app_layer.hpp"
#include "dls2/util/log/log.hpp"
#include <unistd.h>

// TODO build this class
namespace dls
{
class HardwareLayer : public AppLayer
{
public:
	HardwareLayer();
	~HardwareLayer();

	Status run() override;
	Status shutdown() override;

	std::string where() override { return "not yet implemented";}

private:
	pid_t xenomotor_pid;
	pid_t xenorostask_pid;
	logging::coutstream scout;
};
} // end namespace dls

#endif /* end of include guard: HARDWARE_LAYER_HPP_64INVL3T */
