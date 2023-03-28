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
#include "dls2/msg_wrappers/point.hpp"

namespace dls
{
    Point::Point()
    { }

    Point::Point(Point& from)
        : timestamp(from.timestamp)
        , x(from.x)
        , y(from.y)
        , z(from.z)
    { }

    Point::~Point()
    { }

    Point::operator PointMsg() const
    {
        PointMsg msg;

        msg.timestamp(this->timestamp);

        msg.x(this->x);
        msg.y(this->y);
        msg.z(this->z);

        return msg;
    }

    Point& Point::operator=(const PointMsg& msg){

        this->timestamp = msg.timestamp();

        this->x = msg.x();
        this->y = msg.y();
        this->z = msg.z();

        return *this;
    }

    Point& Point::operator=(const Point& from)
    { 
        this->timestamp = from.timestamp;

        this->x = from.x;
        this->y = from.y;
        this->z = from.z;
        
        return *this;
    }
} // end namespace dls