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
********************************************************************************
* Author:            Hendrik de Bruin                                          *
* Maintainer:        Hendrik de Bruin                                          *
* author email:      hendrik.debruin@iit.it                                    *
*******************************************************************************/
#include "application_framework/console_layer.hpp"
#include <iostream>
#include <sstream>

#include "topics/activate_controller.hpp"
#include "topics/deactivate_controller.hpp"
#include "topics/activate_gait_generator.hpp"
#include "topics/deactivate_gait_generator.hpp"
// =============================================================================
// Constructors
// =============================================================================
ConsoleLayer::ConsoleLayer() :
	pub_activate_controller(topics::activate_controller),
	pub_deactivate_controller(topics::deactivate_controller),
	pub_activate_gait_generator(topics::activate_gait_generator),
	pub_deactivate_gait_generator(topics::deactivate_gait_generator)
{

}

// =============================================================================
// Interface Override Functions
// =============================================================================
ConsoleLayer::Status ConsoleLayer::run()
{

	std::string str;

	while(std::getline(std::cin, str))
	{
		std::stringstream ss(str);

		std::string command;
		ss >> command;

		std::string arg;
		ss >> arg;

		// std::cout << "Command: '" << command <<
		// 	"'\nArg: '" << arg << "'\n" << std::endl;

		StringMsg msg; msg.msg(arg);
		if(command == "activateController")
		{
			pub_activate_controller.publish(msg);

		}
		else if(command == "deactivateController")
		{
			pub_deactivate_controller.publish(msg);
		}
		else if(command == "activateGaitGenerator")
		{
			pub_activate_gait_generator.publish(msg);
		}
		else if(command == "deactivateGaitGenerator")
		{
			pub_deactivate_gait_generator.publish(msg);
		}
		else
		{
			std::cerr << "Command '" << command << "' not recognized" << std::endl;
		}
	}
	std::cout << "console EXIT" << std::endl;

	return getStatus();
}

ConsoleLayer::Status ConsoleLayer::shutdown()
{

	return getStatus();
}
