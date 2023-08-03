
#ifndef VICON_HPP
#define VICON_HPP

#include "dls2/msg_wrappers/wrapper.hpp"
#include "dls_messages/dds/vicon.h"
#include <Eigen/Dense>

namespace dls
{
    class Vicon : public Wrapper<ViconMsg>
    {
    public:
        Vicon();
        Vicon(Vicon& vicon);
        virtual ~Vicon();

        operator ViconMsg() const override;
        Vicon& operator=(const ViconMsg& vicon_msg) override;
        Vicon& operator=(const Vicon& vicon);

		std::string frame_id_{};
		uint32_t sequence_id_{};
		double timestamp_{};

        Eigen::Vector3d robot_position_{Eigen::Vector3d::Zero()};
        Eigen::Quaterniond robot_orientation_{Eigen::Quaterniond::Identity()};
        // EDIT#3/7 use it when PlotJuggler plugin for FastDDS supports sequence data structures
        // std::vector<Eigen::Vector3d> markers_positions{};
        std::array<double, 21> markers_positions_{};
    };
} // namespace dls
#endif