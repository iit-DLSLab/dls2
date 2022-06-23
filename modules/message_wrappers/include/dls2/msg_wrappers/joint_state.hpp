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
#ifndef JOINT_STATE_HPP_MYSDEPUT
#define JOINT_STATE_HPP_MYSDEPUT

#include <Eigen/Dense>

#include <dls/messages/joint_statePubSubTypes.h>

namespace dls
{
struct JointState
{
	JointState();
	JointState(JointStateMsg&);
	operator JointStateMsg() const;

	Eigen::VectorXd position;
	Eigen::VectorXd velocity;
	Eigen::VectorXd acceleration;
	Eigen::VectorXd effort;
};
} // end namespace dls
#endif /* end of include guard: JOINT_STATE_HPP_MYSDEPUT */
