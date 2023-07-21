
#ifndef VICON_HPP
#define VICON_HPP

#include "dls2/msg_wrappers/wrapper.hpp"
#include "dls_messages/dds/viconPubSubTypes.h"

#include <Eigen/Dense>

namespace dls
{
    class Vicon : public Wrapper<ViconMsg>
    {
    public:
        Vicon();
        Vicon(Vicon&);
        ~Vicon();

        operator ViconMsg() const override;
        Vicon& operator=(const ViconMsg&) override;
        Vicon& operator=(const Vicon&);

        double timestamp{};

        Eigen::Vector3d robot_position{Eigen::Vector3d::Zero()};
        Eigen::Quaterniond robot_orientation;
        // EDIT#3/7 use it when PlotJuggler plugin for FastDDS supports sequence data structures
        // std::vector<Eigen::Vector3d> markers_positions{};
        std::array<double, 21> markers_positions{};
    };
} // end namespace dls
#endif /* end of include guard: VICON_HPP */
