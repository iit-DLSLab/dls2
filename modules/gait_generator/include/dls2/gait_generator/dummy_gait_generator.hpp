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
#ifndef DUMMY_GAIT_GENERATOR_HPP_BVC8KPN1
#define DUMMY_GAIT_GENERATOR_HPP_BVC8KPN1

#include "dls2/gait_generator/gait_generator.hpp"
#include "dls2/log/log.hpp"
#include "dls/messages/stringmsgPubSubTypes.h"
#include "dls2/util/service/service.hpp"

namespace dls
{
// TODO("this is a temporary class. Remove this file, the cpp file, and the CMake entry")
class DummyGaitGenerator : public GaitGenerator
{
public:
	// DummyGaitGenerator();
	DummyGaitGenerator(const std::shared_ptr<iit::dog::Dog> &pDog);
	void run(const std::chrono::system_clock::time_point&) override;
	AppLayerComponent::Status eStop() override {return getStatus();}

	std::string where() override;

private:
	logging::coutstream scout;
	logging::clogstream sclog;
	ServiceClient<StringMsgPubSubType, StringMsgPubSubType> service_client;
};
} // end namespace dls

#endif /* end of include guard: DUMMY_GAIT_GENERATOR_HPP_BVC8KPN1 */
