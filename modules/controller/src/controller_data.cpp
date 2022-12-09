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
#ifndef CONTROLLER_DATA_CPP
#define CONTROLLER_DATA_CPP

#include "dls2/controller/controller_data.hpp"

using namespace dls;

ControllerData::ControllerData
(
	const std::shared_ptr<robotlib::RobotBase> &pRobot,
	std::shared_ptr<math::SplineBase<double>> spline_in_,
	std::shared_ptr<math::SplineBase<double>> spline_out_,
	const std::chrono::duration<double> &duration_in,
	const std::chrono::duration<double> &duration_out
) :
	AppData(),
	premultiplier(0),
	spline_in_duration(duration_in),
	spline_out_duration(duration_out),
	pSpline_in(spline_in_),
	pSpline_out(spline_out_),
	control_signal(pRobot)
{ }


ControlSignal ControllerData::getLastPublishedControlSignal()
{
	std::lock_guard<std::mutex> lock(this->control_signal_mutex);
	return this->control_signal;
}

#endif /* end of include guard: CONTROL_SIGNAL_HPP_QCFRROHM */