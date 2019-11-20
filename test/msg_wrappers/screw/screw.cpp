#include "msg_wrappers/screw.hpp"
#include <iostream>

int main()
{
	Screw s;
	Eigen::Vector3d lin; lin << 0, 1, 2;
	Eigen::Vector3d ang; ang << 3, 4, 5;
	s.setLinear(lin);
	s.setAngular(ang);

	std::cout << "Screw: " << s.data().transpose() << std::endl;

	auto linout = s.getLinear();
	std::cout << "linout: " << linout.transpose() << std::endl;

	if(linout(0) != lin(0))
	{
		return EXIT_FAILURE;
	}
	if(linout(1) != lin(1))
	{
		return EXIT_FAILURE;
	}
	if(linout(2) != lin(2))
	{
		return EXIT_FAILURE;
	}

	auto angout = s.getAngular();
	std::cout << "angout: " << angout.transpose() << std::endl;
	if(angout(0) != ang(0))
	{
		return EXIT_FAILURE;
	}
	if(angout(1) != ang(1))
	{
		return EXIT_FAILURE;
	}
	if(angout(2) != ang(2))
	{
		return EXIT_FAILURE;
	}

	return EXIT_SUCCESS;
}
