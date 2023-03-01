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
	const std::string& ID,
	std::shared_ptr<dls::DDSParticipant> participant,
	const dls::topicType& topic,
	const std::shared_ptr<robotlib::RobotBase> pRobot,
	std::shared_ptr<math::SplineBase<double>> spline_in_,
	std::shared_ptr<math::SplineBase<double>> spline_out_,
	const std::chrono::duration<double> &duration_in,
	const std::chrono::duration<double> &duration_out
) 
	: AppData(ID)
	, premultiplier(0)
	, spline_in_duration(duration_in)
	, spline_out_duration(duration_out)
	, pSpline_in(spline_in_)
	, pSpline_out(spline_out_)
	, control_signal(
		participant, 
		topic,
		std::make_shared<ControlSignal>(pRobot)
	)
{ }

#endif /* end of include guard: CONTROL_SIGNAL_HPP_QCFRROHM */