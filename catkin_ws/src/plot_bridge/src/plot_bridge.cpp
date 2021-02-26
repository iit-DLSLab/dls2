#include "ros/ros.h"
#include "plot_bridge/joint_state.h"
#include "plot_bridge/total_desired_torque.h"

#include "dls2/util/messaging/callback_subscriber.hpp"
#include "dls2/msg/desired_torquesPubSubTypes.h"

#include "dls2/topics/desired_torques.hpp"

#include <iostream>

int main(int argc, char **argv)
{
	ros::init(argc, argv, "plot_bridge");
	ros::NodeHandle n;

	dls::CallbackSubscriber<DesiredTorquesMsgPubSubType> desired_torques
	(
		dls::topics::desired_torques,
		[&](DesiredTorquesMsg msg)
		{
			static ros::Publisher desired_torque_publisher =
				n.advertise<plot_bridge::total_desired_torque>
				(
					"desired_torques",
					1000
				);

			plot_bridge::total_desired_torque ros_msg;
			for(const auto &el : msg.desired_torques())
			{
				ros_msg.torques.push_back(el);
			}

			desired_torque_publisher.publish(ros_msg);

			static size_t i = 0;
			std::cout << "publish " << ++i << std::endl;
		}
	);

	std::cin.ignore();

	return EXIT_SUCCESS;
}
