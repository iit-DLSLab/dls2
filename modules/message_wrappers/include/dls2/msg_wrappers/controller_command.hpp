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

#ifndef CONTROLLER_COMMAND_HPP
#define CONTROLLER_COMMAND_HPP

#include <robotlib/robot_base.hpp>

namespace Eigen
{
    typedef Eigen::Matrix<double,6,1> Vector6d;
}

namespace dls
{
    class ControllerCommand {
        public:
            ControllerCommand(const std::shared_ptr<robotlib::RobotBase>& robot);
            ControllerCommand() = delete;
            ~ControllerCommand();
            
            double robot_height;
            double step_frequency;
            double duty_factor;
            robotlib::LegDataMap<double> step_height;
            Eigen::Vector6d des_base_pos_HF;
            Eigen::Vector6d des_base_vel_HF;
    };
}
#endif // CONTROLLER_COMMANDS_HPP
