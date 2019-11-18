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
// TODO this is a tool that needs to be made into a separate project and made
// more sophisticated
#ifndef TOPIC_INFO_HPP_SVMAPUJA
#define TOPIC_INFO_HPP_SVMAPUJA

#include "todo.h"
#include "util/messaging/subscriber_base.hpp"
#include "msg/blind_statePubSubTypes.h"
#include "msg/gait_signalPubSubTypes.h"
#include "msg/control_signalPubSubTypes.h"

class TopicInfo
{
public:
	TopicInfo(const std::string &controller = "dls_dummy_controller");

private:
	class BlindStateSub : public SubscriberBase<BlindStateMsgPubSubType>
	{
	public:
		BlindStateSub();
	private:
		void onNewDataMessage(eprosima::fastrtps::Subscriber *sub) override;
		eprosima::fastrtps::SampleInfo_t info;
	}blind_state_sub;

	class GaitSignalSub : public SubscriberBase<GaitSignalMsgPubSubType>
	{
	public:
		GaitSignalSub();
	private:
		void onNewDataMessage(eprosima::fastrtps::Subscriber *sub) override;
		eprosima::fastrtps::SampleInfo_t info;
	}gait_signal_sub;

	class ControlSignalSub : public SubscriberBase<ControlSignalMsgPubSubType>
	{
	public:
		ControlSignalSub(const std::string&);
	private:
		void onNewDataMessage(eprosima::fastrtps::Subscriber *sub) override;
		eprosima::fastrtps::SampleInfo_t info;
	}control_signal_sub;
};

#endif /* end of include guard: TOPIC_INFO_HPP_SVMAPUJA */
