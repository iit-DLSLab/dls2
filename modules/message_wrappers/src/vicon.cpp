
#include "dls2/msg_wrappers/vicon.hpp"

Vicon::Vicon()
    : frame_id_("")
	, sequence_id_(0)
	, timestamp_(0.0)
    , robot_position_(Eigen::Vector3d::Zero())
    , robot_orientation_(Eigen::Quaterniond::Identity())
    , markers_positions_()
{}

Vicon::Vicon(Vicon& vicon)
    : frame_id_(vicon.frame_id_)
	, sequence_id_(vicon.sequence_id_)
	, timestamp_(vicon.timestamp_)
    , robot_position_(vicon.robot_position_)
    , robot_orientation_(vicon.robot_orientation_)
    , markers_positions_(vicon.markers_positions_)
{}

Vicon::~Vicon(){}

Vicon::operator ViconMsg() const
{
    ViconMsg vicon_msg;

	vicon_msg.frame_id(frame_id_);
	vicon_msg.sequence_id(sequence_id_);
	vicon_msg.timestamp(timestamp_);

    vicon_msg.robot_position()[0] = robot_position_[0];
    vicon_msg.robot_position()[1] = robot_position_[1];
    vicon_msg.robot_position()[2] = robot_position_[2];

    vicon_msg.robot_orientation()[0] = robot_orientation_.x();
    vicon_msg.robot_orientation()[1] = robot_orientation_.y();
    vicon_msg.robot_orientation()[2] = robot_orientation_.z();
    vicon_msg.robot_orientation()[3] = robot_orientation_.w();

    // EDIT#4/7 use it when PlotJuggler plugin for FastDDS supports sequence data structures
    //for(unsigned int marker_index{0}; marker_index < this->markers_positions.size(); ++marker_index)
    //{
    //    PositionMsg marker;
    //
    //    marker.x() = this->markers_positions.at(marker_index)[0];
    //    marker.y() = this->markers_positions.at(marker_index)[1];
    //    marker.z() = this->markers_positions.at(marker_index)[2];
    //
    //    vicon_msg.markers_positions().push_back(marker);
    //}

    for(unsigned int marker_index{0}; marker_index<markers_positions_.size(); ++marker_index)
    {
        // vicon_msg.markers_positions().push_back(this->markers_positions.at(marker_index));
        vicon_msg.markers_positions()[marker_index] = markers_positions_[marker_index];
    }

    return vicon_msg;
}

Vicon& Vicon::operator=(const ViconMsg& vicon_msg)
{
	frame_id_ = vicon_msg.frame_id();
	sequence_id_ = vicon_msg.sequence_id();
	timestamp_ = vicon_msg.timestamp();

    robot_position_[0] = vicon_msg.robot_position()[0];
    robot_position_[1] = vicon_msg.robot_position()[1];
    robot_position_[2] = vicon_msg.robot_position()[2];

    robot_orientation_.x() = vicon_msg.robot_orientation()[0];
    robot_orientation_.y() = vicon_msg.robot_orientation()[1];
    robot_orientation_.z() = vicon_msg.robot_orientation()[2];
    robot_orientation_.w() = vicon_msg.robot_orientation()[3];

    // EDIT#5/7 use it when PlotJuggler plugin for FastDDS supports sequence data structures
    //for(unsigned int marker_index{0}; marker_index < vicon_msg.markers_positions().size(); ++marker_index)
    //{
    //    Eigen::Vector3d marker{vicon_msg.markers_positions().at(marker_index).x(),
    //                           vicon_msg.markers_positions().at(marker_index).y(),
    //                           vicon_msg.markers_positions().at(marker_index).z()};
    //
    //    this->markers_positions.push_back(marker);
    //}

    for(unsigned int marker_index{0}; marker_index<vicon_msg.markers_positions().size(); ++marker_index)
    {
        // this->markers_positions.push_back(vicon_msg.markers_positions().at(marker_index));
        markers_positions_[marker_index] = vicon_msg.markers_positions()[marker_index];
    }

    return *this;
}

Vicon& Vicon::operator=(const Vicon& vicon)
{
    frame_id_ = vicon.frame_id_;
	sequence_id_ = vicon.sequence_id_;
	timestamp_ = vicon.timestamp_;

    robot_position_ = vicon.robot_position_;
    robot_orientation_ = vicon.robot_orientation_;

    // EDIT#6/7 use it when PlotJuggler plugin for FastDDS supports sequence data structures
    //for(unsigned int marker_index{0}; marker_index < from.markers_positions.size(); ++marker_index)
    //{
    //    this->markers_positions.at(marker_index)[0] = from.markers_positions.at(marker_index)[0];
    //    this->markers_positions.at(marker_index)[1] = from.markers_positions.at(marker_index)[1];
    //    this->markers_positions.at(marker_index)[2] = from.markers_positions.at(marker_index)[2];
    //}

    for(unsigned int marker_index{0}; marker_index < vicon.markers_positions_.size(); ++marker_index)
    {
        markers_positions_.at(marker_index) = vicon.markers_positions_.at(marker_index);
    }

    return *this;
}