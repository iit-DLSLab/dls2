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
#ifndef CONSOLE_LAYER_HPP_BXNRZS8Q
#define CONSOLE_LAYER_HPP_BXNRZS8Q

#include "application_framework/app_layer.hpp"

// messaging
#include "util/messaging/publisher_base.hpp"
#include "msg/stringmsgPubSubTypes.h"

class ConsoleLayer : public AppLayer
{
public:
	ConsoleLayer();
	~ConsoleLayer() = default;

	Status run() override;
	Status shutdown() override;

private:
	TODO("These publishers need to be put into a better structure")
	PublisherBase<StringMsgPubSubType> pub_activate_controller;
	PublisherBase<StringMsgPubSubType> pub_deactivate_controller;
	PublisherBase<StringMsgPubSubType> pub_activate_gait_generator;
	PublisherBase<StringMsgPubSubType> pub_deactivate_gait_generator;
};

#endif /* end of include guard: CONSOLE_LAYER_HPP_BXNRZS8Q */
