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
#include <iostream>
#include <cstdlib>

#include "geometry/pose.hpp"

TODO("Use a proper testing framework")
int main(int argc, char *argv[])
{
	Eigen::Vector3d pos;
	pos << 20, 21, 22;
	Eigen::Quaterniond quat(10, 11, 12, 13);

	dls::Pose p(pos, quat);

	PoseMsg pm = p;

// =============================================================================
// Test Conversion to message
// =============================================================================
	std::cout << "pose-msg position comparison" << std::endl;
	for(int i = 0; i != 3; ++i)
	{
		std::cout << p.toPosition()(i) << " " << pm.position()[i] << std::endl;
		if(pm.position()[i] != p.toPosition()(i))
		{
			std::cerr << "PoseMessage.position " << i <<
				" does not correspond to original" << std::endl;
			return EXIT_FAILURE;
		}
	}

	std::cout << "pose-msg quaternion comparison" << std::endl;
	for(int i = 0; i != 4; ++i)
	{
		std::cout << p.toQuaternion().coeffs()[i] << " " << pm.quaternion()[i] << std::endl;
		if(pm.quaternion()[i] != p.toQuaternion().coeffs()[i])
		{
			std::cerr << "quaternion component " << i
				<< " does not match" << std::endl;
			return EXIT_FAILURE;
		}
	}


// =============================================================================
// Test conversion back to pose
// =============================================================================
	dls::Pose p2(pm);
	std::cout << "Pose-pose position comparison" << std::endl;
	for(int i = 0; i != 3; ++i)
	{
		std::cout << p.toPosition()(i) << " " << p2.toPosition()(i) << std::endl;
		if(p2.toPosition()(i) != p.toPosition()(i))
		{
			std::cerr << "Pose 2 position component " << i
				<< " does not match original pose" << std::endl;
			return EXIT_FAILURE;
		}
	}

	std::cout << "Pose-pose quaternion comparison" << std::endl;
	for(int i = 0; i != 4; ++i)
	{
		std::cout << p.toQuaternion().coeffs()[i] << " " << p2.toQuaternion().coeffs()[i] << std::endl;
		if(p2.toQuaternion().coeffs()[i] != p.toQuaternion().coeffs()[i])
		{
			std::cerr << "Pose 2 quaternion component " << i
				<< " does not match original pose" << std::endl;
			return EXIT_FAILURE;
		}
	}

	std::cout << "test passed" << std::endl;
	return EXIT_SUCCESS;
}
