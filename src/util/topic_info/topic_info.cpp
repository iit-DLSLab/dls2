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
#include "util/topic_info/topic_info.hpp"

#include "topics/gait_signal.hpp"
#include "topics/low_level_estimation/blind_state.hpp"

#include <algorithm>
// =============================================================================
// Constructors
// =============================================================================
TopicInfo::TopicInfo() :
	blind_state_sub(),
	gait_signal_sub()
{ }

// =============================================================================
// Subscribers
// =============================================================================
// -----------------------------------------------------------------------------
// Blind State
// -----------------------------------------------------------------------------
TopicInfo::BlindStateSub::BlindStateSub() :
	SubscriberBase<BlindStateMsgPubSubType>(topics::low_level_estimation::blind_state),
	info()
{ }

void TopicInfo::BlindStateSub::onNewDataMessage
(
	eprosima::fastrtps::Subscriber *sub
)
{
	BlindStateMsg msg;
	if(sub->takeNextData((void*)&msg, &info))
	{
		if(info.sampleKind == eprosima::fastrtps::rtps::ALIVE)
		{
			std::cout << "=========" << std::endl;
			std::cout << "Blind state\n";
				// ======================== Joint state ========================
				std::cout << "\tJoint position: ";
					std::for_each
						(
							msg.joint_state().position().cbegin(),
							msg.joint_state().position().cend(),
							[](double d){std::cout << d << " ";}
						);
					std::cout << "\n"
					<< "\tJoint velocity: ";
					std::for_each
						(
							msg.joint_state().velocity().cbegin(),
							msg.joint_state().velocity().cend(),
							[](double d){std::cout << d << " ";}
						);
					std::cout << "\n"
					<< "\tJoint effort: ";
					std::for_each
						(
							msg.joint_state().effort().cbegin(),
							msg.joint_state().effort().cend(),
							[](double d){std::cout << d << " ";}
						);
					std::cout << "\n";

				// ========================= Body Pose =========================
				std::cout << "Body Pose\n";
					std::cout << "\tPosition";
						std::for_each
							(
								msg.body_pose().position().cbegin(),
								msg.body_pose().position().cend(),
								[](double d){std::cout << d << " ";}
							);
					std::cout << "\n";
					std::cout << "\tQuaternion";
						std::for_each
							(
								msg.body_pose().quaternion().cbegin(),
								msg.body_pose().quaternion().cend(),
								[](double d){std::cout << d << " ";}
							);

				// ========================= Velocity ==========================
				std::cout << "Body velocity\n";
					std::cout << "\tLinear: ";
						std::for_each
							(
								msg.body_velocity().linear().cbegin(),
								msg.body_velocity().angular().cend(),
								[](double d){std::cout << d << " ";}
							);
					std::cout << "\n";

				// ======================= Acceleration ========================
				std::cout << "Body acceleration\n";
					std::cout << "\tLinear: ";
						std::for_each
							(
								msg.body_acceleration().linear().cbegin(),
								msg.body_acceleration().angular().cend(),
								[](double d){std::cout << d << " ";}
							);
					std::cout << "\n";
				std::cout << "=========" << std::endl;


			std::cout << std::endl;
		}
	}
}

// -----------------------------------------------------------------------------
// Gait Signal
// -----------------------------------------------------------------------------
TopicInfo::GaitSignalSub::GaitSignalSub() :
	SubscriberBase<GaitSignalMsgPubSubType>(topics::gait_signal),
	info()
{ }

void TopicInfo::GaitSignalSub::onNewDataMessage
(
	eprosima::fastrtps::Subscriber *sub
)
{
	GaitSignalMsg msg;
	if(sub->takeNextData((void*)&msg, &info))
	{
		if(info.sampleKind == eprosima::fastrtps::rtps::ALIVE)
		{
				// ========================= CoM Pose ==========================
				std::cout << "=========" << std::endl;
				std::cout << "Desired com pose\n";
					std::cout << "\tPosition ";
						std::for_each
							(
								msg.desired_com_pose().position().cbegin(),
								msg.desired_com_pose().position().cend(),
								[](double d){std::cout << d << " ";}
							);
					std::cout << "\n";
					std::cout << "\tQuaternion ";
						std::for_each
							(
								msg.desired_com_pose().quaternion().cbegin(),
								msg.desired_com_pose().quaternion().cend(),
								[](double d){std::cout << d << " ";}
							);
					std::cout << "\n";

				// ========================= Base Pose =========================
				std::cout << "Desired base pose\n";
					std::cout << "\tPosition ";
						std::for_each
							(
								msg.desired_base_pose().position().cbegin(),
								msg.desired_base_pose().position().cend(),
								[](double d){std::cout << d << " ";}
							);
					std::cout << "\n";
					std::cout << "\tQuaternion ";
						std::for_each
							(
								msg.desired_base_pose().quaternion().cbegin(),
								msg.desired_base_pose().quaternion().cend(),
								[](double d){std::cout << d << " ";}
							);
					std::cout << "\n";
				std::cout << "=========" << std::endl;

		}
	}
}

// TopicInfo::ControlSignalSub::ControlSignalSub() :
// 	SubscriberBase<ControlSignalMsgPubSubType>(topics::control_signal)
// { }

