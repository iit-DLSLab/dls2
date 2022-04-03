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
#include "dls2/core_framework/gait_generator_layer.hpp"
#include "dls2/topics/gait_signal.hpp"

using namespace dls;


GaitLayer::GaitLayer() :
	AppLayer("gait_layer"),

	// publisher publishes to "gait_layer" topic
	pub(dls::topics::gait_layer),

	// Subscriber listens for "gait_signal" as published by gait generators
	sub
	(
		dls::topics::gait_signal,
		[&](GaitSignalMsg msg)
		{
			this->pub.publish(msg);
		}
	)
{ }
