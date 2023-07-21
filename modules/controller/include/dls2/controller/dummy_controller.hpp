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
#ifndef DUMMY_CONTROLLER_HPP_YGHMJHQN
#define DUMMY_CONTROLLER_HPP_YGHMJHQN

#include <iostream>

#include "dls2/controller/controller.hpp"
#include "dls2/util/service/service_server.hpp"
#include "dls2/command/command.hpp"
#include "dls2/log/log.hpp"

#include "dls_messages/dds/stringmsgPubSubTypes.h"

#include <iit/commons/dog/dog.hpp>

// TODO This is a dummy class. Remove it, its cpp file, as well as its entry in the CMakeLists file
namespace dls
{
class DummyController : public Controller
{
public:
	DummyController
	(
		const std::shared_ptr<robotlib::RobotBase> &dog
	);

	// DummyController();

	~DummyController();

	void run(const std::chrono::system_clock::time_point &time) override;

	// AppStatus stop() override { return getStatus(); }
	AppStatus eStop() override { return getStatus(); }

	std::string where() override;

private:
	// CommandManager command_manager;
	// logging::clogstream outstream;
	// logging::clogstream logstream;

	// ServiceServer<StringMsgPubSubType, StringMsgPubSubType> service_server_;
};
} // end namespace dls

#endif /* end of include guard: DUMMY_CONTROLLER_HPP_YGHMJHQN */
