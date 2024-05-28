#include "dls2/msg_wrappers/blind_state.hpp"
#include "dls2/msg_wrappers/base_state.hpp"
#include "robotlib/robot_factory.hpp"

#include <iostream>
#include <signal.h>
#include <stdlib.h>
#include <stdio.h>
#include <unistd.h>

int main(int argc, char** argv)
{
	static_cast<void> (argc);
	static_cast<void> (argv);

	auto robot = robotlib::RobotFactory::openRobot("aliengo");
	dls::BlindState blind_state(robot);
	dls::BlindState blind_state2(robot);
	dls::BaseState base_state(robot);
	dls::BaseState base_state2(robot);
	blind_state.frame_id_ = "frame 1";
	base_state.frame_id_ = "frame 1";
	blind_state2.frame_id_ = "frame 2";
	base_state2.frame_id_ = "frame 2";

	// WrapperBase *blind_state_wb = &blind_state;
	// WrapperBase *blind_state2_wb = &blind_state2;
	// WrapperBase *base_state_wb = &base_state;
	// WrapperBase *base_state2_wb = &base_state2;

	// blind_state_wb->setDataFromWrapperBase(blind_state2_wb);
	// base_state_wb->setDataFromWrapperBase(base_state2_wb);

	std::cout << blind_state.frame_id_ << " " << base_state.frame_id_ << std::endl;

	return EXIT_SUCCESS;
}