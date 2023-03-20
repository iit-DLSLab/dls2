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
        , position(from.position)
        , orientation(from.orientation)
    { }

    Vicon::~Vicon()
    { }

    Vicon::operator ViconMsg() const
    {
        ViconMsg msg;

        msg.timestamp(this->timestamp);

        msg.position()[0] = this->position[0];
        msg.position()[1] = this->position[1];
        msg.position()[2] = this->position[2];

        msg.orientation()[0] = this->orientation.x();
        msg.orientation()[1] = this->orientation.y();
        msg.orientation()[2] = this->orientation.z();
        msg.orientation()[3] = this->orientation.w();

        return msg;
    }

    Vicon& Vicon::operator=(const ViconMsg& msg){

        this->timestamp = msg.timestamp();

        this->position[0] = msg.position()[0];
        this->position[1] = msg.position()[1];
        this->position[2] = msg.position()[2];

        this->orientation.x() = msg.orientation()[0];
        this->orientation.y() = msg.orientation()[1];
        this->orientation.z() = msg.orientation()[2];
        this->orientation.w() = msg.orientation()[3];

        return *this;
    }

    Vicon& Vicon::operator=(const Vicon& from)
    { 
        this->timestamp = from.timestamp;
        this->position = from.position;
        this->orientation = from.orientation;
        
        return *this;
    }
} // end namespace dls