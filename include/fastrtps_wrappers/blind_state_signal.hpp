#ifndef BLIND_STATE_SIGNAL_HPP
#define BLIND_STATE_SIGNAL_HPP

#include "geometry/pose.hpp"
#include "todo.h"
#include <Eigen/Dense>
#include "msg/blind_state.h"

struct BlindStateSignal
{
	BlindStateSignal();
	BlindStateSignal(BlindStateMsg);
	operator BlindStateMsg() const;

	Eigen::VectorXd joint_position;			///< The joint position
	Eigen::VectorXd joint_velocity;			///< The joint velocity
	Eigen::VectorXd joint_effort;			///< The joint effort

};

#endif /* end of include guard: BLIND_STATE_SIGNAL_HPP */
