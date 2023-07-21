
#ifndef T265_STATE_HPP
#define T265_STATE_HPP

#include "robotlib/robot_base.hpp"

#include "dls2/msg_wrappers/wrapper.hpp"
// #include "dls2/msg_wrappers/pose.hpp"
// #include "dls2/msg_wrappers/screw.hpp"
#include "dls_messages/dds/t265_state.h"

namespace dls
{
	class T265State : public Wrapper<T265StateMsg>
	{
	public:
		T265State();
		T265State(T265State&);
        ~T265State();

		operator T265StateMsg() const override;
		T265State& operator= (const T265StateMsg&) override;
		T265State& operator= (const T265State&);

		std::string robot_name;

        Eigen::Vector3d position;
        Eigen::Quaterniond orientation;

        Eigen::Vector3d linear_velocity;
        Eigen::Vector3d angular_velocity;
        double timestamp;
	};
} // end namespace dls

#endif /* end of include guard: T265_STATE_HPP */
