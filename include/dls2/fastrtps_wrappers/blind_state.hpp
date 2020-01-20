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
#ifndef BLIND_STATE_SIGNAL_HPP
#define BLIND_STATE_SIGNAL_HPP

//TODO move this file out of fastrtps_wrappers and into msg_wrappres

#include "dls2/geometry/pose.hpp"
#include "dls2/msg/blind_state.h"
#include "dls2/msg_wrappers/joint_state.hpp"
#include "dls2/msg_wrappers/screw.hpp"
#include <Eigen/Dense>

namespace dls
{
struct BlindState
{
	BlindState();
	// TODO should this be by const reference?
	BlindState(BlindStateMsg);
	operator BlindStateMsg() const;

	JointState joint_state;
	Pose base_pose_world;
	Screw base_velocity_world;
	Screw base_acceleration_world;
	double time;
};
} // end namespace dls

#endif /* end of include guard: BLIND_STATE_SIGNAL_HPP */
