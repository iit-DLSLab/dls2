
#ifndef FOOT_STATE_CPP
#define FOOT_STATE_CPP

#include "dls2/msg_wrappers/foot_state.hpp"

using namespace dls;

FootState::FootState()
    : pos(Eigen::Vector3d::Zero())
    , pos_HF(Eigen::Vector3d::Zero())
    , vel(Eigen::Vector3d::Zero())
    , vel_HF(Eigen::Vector3d::Zero())
    , acc(Eigen::Vector3d::Zero())
{}

FootState::~FootState()
{}

FootState FootState::Zero()
{
    return FootState();
}

#endif // FOOT_STATE_CPP