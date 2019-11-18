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
#ifndef ROSCONTROL_TO_DLS2_HPP_X8TSOKXU
#define ROSCONTROL_TO_DLS2_HPP_X8TSOKXU

<<<<<<< HEAD
// Ros subscription
#include <message_filters/subscriber.h>
#include <message_filters/time_synchronizer.h>
#include <geometry_msgs/Pose.h>
#include <geometry_msgs/Twist.h>
#include <sensor_msgs/JointState.h>

// Fastrtps publishing
#include "util/messaging/publisher_base.hpp"
#include "msg/blind_statePubSubTypes.h"

namespace roscontrol_to_dls2
{

class RoscontrolToDls2
{
public:
	RoscontrolToDls2();

private:
	ros::NodeHandle ros_node;
	PublisherBase<BlindStateMsgPubSubType> state_pub;

	message_filters::Subscriber<JointState> joint_state_sub;
	message_filters::Subscriber<Pose> pose_sub;
	message_filters::Subscriber<Twist> velocity_sub;
	message_filters::Subscriber<Twist> acceleration_sub;

};

} // namespace roscontrol_to_dls2
=======
//#include "util/messaging/publisher_base.hpp"
//#include "msg/blind_state.h"

namespace roscontrol_to_dls2
{
	class RoscontrolToDls2
	{
	public:
		RoscontrolToDls2();

	private:
		//PublisherBase<BlindStateMsgPubSubType> state_pub;
	};
}
>>>>>>> 8dbd1c57921f616aaf1ab47727de16fd5748311c

#endif /* end of include guard: ROSCONTROL_TO_DLS2_HPP_X8TSOKXU */
