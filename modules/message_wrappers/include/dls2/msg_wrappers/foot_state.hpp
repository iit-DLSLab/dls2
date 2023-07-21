
#ifndef FOOT_STATE_HPP
#define FOOT_STATE_HPP

#include <robotlib/robot_base.hpp>

namespace dls
{
    class FootState {
    public:
        FootState();
        ~FootState();

        static FootState Zero();

        Eigen::Vector3d pos;
        Eigen::Vector3d pos_HF;
        Eigen::Vector3d vel;
        Eigen::Vector3d vel_HF;
        Eigen::Vector3d acc;
    };
}
#endif // FOOT_STATE_HPP