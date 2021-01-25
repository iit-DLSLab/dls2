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
#ifndef GAIT_GENERATOR_LAYER_PHPP_6bf82ff6_8192_4ed8_abd5_ac9d0b5328c1
#define GAIT_GENERATOR_LAYER_PHPP_6bf82ff6_8192_4ed8_abd5_ac9d0b5328c1
// =============================================================================
// Includes
// =============================================================================
#include "dls2/application_framework/app_layer.hpp"
#include "dls2/util/messaging/publisher_base.hpp"
#include "dls2/util/messaging/callback_subscriber.hpp"

#include "dls2/msg/gait_signalPubSubTypes.h"

namespace dls
{
	class GaitLayer : public AppLayer
	{
	public:
		GaitLayer();

	private:
		PublisherBase<GaitSignalMsgPubSubType> pub;
		CallbackSubscriber<GaitSignalMsgPubSubType> sub;
	};
}

#endif // GAIT_GENERATOR_LAYER_PHPP_6bf82ff6_8192_4ed8_abd5_ac9d0b5328c1
