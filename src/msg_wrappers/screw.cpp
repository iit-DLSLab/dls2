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
#include "msg_wrappers/screw.hpp"

// =============================================================================
// Constructors
// =============================================================================
Screw::Screw():
	eigen_vec()
{ }

Screw::Screw(const ScrewMsg &msg)
{
	this->eigen_vec <<

		msg.linear()[0],
		msg.linear()[1],
		msg.linear()[2],

		msg.angular()[0],
		msg.angular()[1],
		msg.angular()[2];
}

// =============================================================================
// Conversions
// =============================================================================
Screw::operator ScrewMsg() const
{
	ScrewMsg msg;
	msg.linear()[0] = this->eigen_vec(0);
	msg.linear()[1] = this->eigen_vec(1);
	msg.linear()[2] = this->eigen_vec(2);
	msg.angular()[0] = this->eigen_vec(3);
	msg.angular()[1] = this->eigen_vec(4);
	msg.angular()[2] = this->eigen_vec(5);
	return msg;
}


// =============================================================================
// Implementation
// =============================================================================
void Screw::setLinear(const Eigen::Vector3d &vec)
{
	this->eigen_vec.block<3, 1>(0, 0) = vec;
}

void Screw::setAngular(const Eigen::Vector3d &vec)
{
	this->eigen_vec.block<3, 1>(3, 0) = vec;
}

Eigen::Matrix<double, 6, 1> &Screw::data()
{
	return this->eigen_vec;
}

Eigen::Vector3d Screw::getLinear() const
{
	return this->eigen_vec.block<3, 1>(0, 0);
}

Eigen::Vector3d Screw::getAngular() const
{
	return this->eigen_vec.block<3, 1>(3, 0);
}
