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
*******************************************************************************/
#include "dls2/msg_wrappers/vicon.hpp"

namespace dls
{
    Vicon::Vicon()
    { }

    Vicon::Vicon(Vicon& from)
        : timestamp(from.timestamp)
        , robot_position(from.robot_position)
        , robot_orientation(from.robot_orientation)
        , markers_positions(from.markers_positions)
    { }

    Vicon::~Vicon()
    { }

    Vicon::operator ViconMsg() const
    {
        ViconMsg msg;

        msg.timestamp(this->timestamp);

        msg.robot_position()[0] = this->robot_position[0];
        msg.robot_position()[1] = this->robot_position[1];
        msg.robot_position()[2] = this->robot_position[2];

        msg.robot_orientation()[0] = this->robot_orientation.x();
        msg.robot_orientation()[1] = this->robot_orientation.y();
        msg.robot_orientation()[2] = this->robot_orientation.z();
        msg.robot_orientation()[3] = this->robot_orientation.w();

        for(unsigned int marker_index{0}; marker_index < this->markers_positions.size(); ++marker_index)
        {
            PointMsg marker;

            marker.x() = this->markers_positions.at(marker_index)[0]; 
            marker.y() = this->markers_positions.at(marker_index)[1]; 
            marker.z() = this->markers_positions.at(marker_index)[2]; 

            msg.markers_positions().push_back(marker);
        }

        return msg;
    }

    Vicon& Vicon::operator=(const ViconMsg& msg)
    {
        this->timestamp = msg.timestamp();

        this->robot_position[0] = msg.robot_position()[0];
        this->robot_position[1] = msg.robot_position()[1];
        this->robot_position[2] = msg.robot_position()[2];

        this->robot_orientation.x() = msg.robot_orientation()[0];
        this->robot_orientation.y() = msg.robot_orientation()[1];
        this->robot_orientation.z() = msg.robot_orientation()[2];
        this->robot_orientation.w() = msg.robot_orientation()[3];

        for(unsigned int marker_index{0}; marker_index < msg.markers_positions().size(); ++marker_index)
        {
            Eigen::Vector3d marker{msg.markers_positions().at(marker_index).x(),
                                   msg.markers_positions().at(marker_index).y(),
                                   msg.markers_positions().at(marker_index).z()};

            this->markers_positions.push_back(marker);
        }

        return *this;
    }

    Vicon& Vicon::operator=(const Vicon& from)
    {
        this->timestamp = from.timestamp;

        this->robot_position = from.robot_position;
        this->robot_orientation = from.robot_orientation;

        for(unsigned int marker_index{0}; marker_index < from.markers_positions.size(); ++marker_index)
        {
            this->markers_positions.at(marker_index)[0] = from.markers_positions.at(marker_index)[0];
            this->markers_positions.at(marker_index)[1] = from.markers_positions.at(marker_index)[1];
            this->markers_positions.at(marker_index)[2] = from.markers_positions.at(marker_index)[2];
        }

        return *this;
    }
} // end namespace dls