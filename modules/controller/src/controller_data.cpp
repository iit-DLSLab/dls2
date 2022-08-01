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
	std::shared_ptr<spline::SplineBase<double>> spline_in_,
	std::shared_ptr<spline::SplineBase<double>> spline_out_,
	const std::chrono::duration<double> &duration_in,
	const std::chrono::duration<double> &duration_out,
	uint controlSize
) :
	proc(nullptr),
	dds_reader(nullptr),
	ID(),
	premultiplier(0),
	spline_in_duration(duration_in),
	spline_out_duration(duration_out),
	pSpline_in(spline_in_),
	pSpline_out(spline_out_),
	control_signal(controlSize)
{ }


ControlSignal ControllerData::getLastPublishedControlSignal()
{
	std::lock_guard<std::mutex> lock(this->control_signal_mutex);
	return this->control_signal;
}

#endif /* end of include guard: CONTROL_SIGNAL_HPP_QCFRROHM */