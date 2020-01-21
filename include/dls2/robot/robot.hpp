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
#ifndef ROBOT_HPP_X9YVSQKM
#define ROBOT_HPP_X9YVSQKM

namespace dls
{
// // TODO remove references to Dog, replace with Robot
// // class Dog {};
// typedef doglib::Dog Dog;

// TODO This class is completely temporary
/// Class representing the robot model
class Robot
{
public:
	// TODO temp field. Joint space dimension
	static const int dimension = 12;

	// TODO Rename this function when the interface of Robot becomes more clear
	// TODO Also make sure this is initialised with the robot library by HyQApp
	static int getDimension(){return Robot::dimension;}
	static int getJointSpaceDimension(){return Robot::dimension;}
};
} // end namespace dls
#endif /* end of include guard: ROBOT_HPP_X9YVSQKM */
