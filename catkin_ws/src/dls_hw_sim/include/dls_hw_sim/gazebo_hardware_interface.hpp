#ifndef GAZEBO_ROS_CONTROL___GAZEBO_HARDWARE_INTERFACE_H
#define GAZEBO_ROS_CONTROL___GAZEBO_HARDWARE_INTERFACE_H

#include <ros/ros.h>
#include <gazebo/physics/physics.hh>

namespace dls_hw_sim
{

	class Interface{
	public:
		void init(gazebo::physics::ModelPtr sim_model) {
			sim_model_ = sim_model;
		}
		virtual void fillAndPublish() = 0;

	protected:
		virtual void fill()=0;
		gazebo::physics::ModelPtr sim_model_;
	};

	template<class msg>
	class RosInterface : public Interface {
	public:
		
		void init(gazebo::physics::ModelPtr sim_model,ros::NodeHandle &model_nh, std::string topic,uint32_t queue_size) {
			Interface::init(sim_model);
			pub_ = model_nh.advertise<msg>(topic,queue_size);
			initMsg();
		}

		virtual void initMsg() {}
		
		virtual void fillAndPublish() override {
			if (pub_.getNumSubscribers()>0)
			{
				fill();
				pub_.publish(msg_);
			}
		}

		msg msg_;

		ros::Publisher pub_;

	};

	class GazeboHardwareInterface {
	public:

		virtual void init (gazebo::physics::ModelPtr sim_model) {
			sim_model_ = sim_model;
		}

		void fillAndPublish() {
			for (auto i : interfaces) {
				i->fillAndPublish();
			}
		}

		void add_interface(Interface *interface) {
			interfaces.push_back(interface);
		}

	protected:

		gazebo::physics::ModelPtr sim_model_;

	private:

		std::vector<Interface*> interfaces;

	};

}

#endif