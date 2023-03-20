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

        Eigen::Vector3d position{Eigen::Vector3d::Zero()};
        Eigen::Quaterniond orientation;        
    };
} // end namespace dls
#endif /* end of include guard: VICON_HPP */
