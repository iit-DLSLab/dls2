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

#include "dls2/msg_wrappers/wrapper.hpp"
#include "dls_messages/dds/controller_command.h"

namespace Eigen
{
    typedef Eigen::Matrix<double,6,1> Vector6d;
}

namespace dls
{
    class ControllerCommand : public Wrapper<ControllerCommandMsg>
    {
        public:
            ControllerCommand(const std::shared_ptr<robotlib::RobotBase>&);
            ControllerCommand() = delete;
            ~ControllerCommand();

            operator ControllerCommandMsg() const override;
		    ControllerCommand& operator= (ControllerCommandMsg&) override;
            
        // private:
            double robot_height;
            double step_frequency;
            double duty_factor;
            robotlib::LegDataMap<double> step_height;
            Eigen::Vector6d des_base_pos_HF;
            Eigen::Vector6d des_base_vel_HF;
    };
}
#endif // CONTROLLER_COMMANDS_HPP
