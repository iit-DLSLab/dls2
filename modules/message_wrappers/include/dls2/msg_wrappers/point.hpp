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
#ifndef POINT_HPP
#define POINT_HPP

#include "dls2/msg_wrappers/wrapper.hpp"
#include "dls_messages/dds/pointPubSubTypes.h"

namespace dls
{
    class Point : public Wrapper<PointMsg>
    {
    public:
        Point();
        Point(Point&);
        ~Point();

        operator PointMsg() const override;
        Point& operator=(const PointMsg&) override;
        Point& operator=(const Point&);

        double timestamp{}, x{}, y{}, z{};        
    };
} // end namespace dls
#endif /* end of include guard: POINT_HPP */
