
#ifndef TRUNK_CONTROLLER_DEBUG_HPP
#define TRUNK_CONTROLLER_DEBUG_HPP


#include "robotlib/robot_base.hpp"

#include "dls2/msg_wrappers/wrapper.hpp"
#include "dls_messages/dds/debug_trunk_controller.h"

namespace dls
{
	class TrunkControllerDebug : public Wrapper<TrunkControllerDebugMsg>
	{
	public:
		TrunkControllerDebug(const std::shared_ptr<robotlib::RobotBase>);
		TrunkControllerDebug(TrunkControllerDebug&);
		TrunkControllerDebug() = delete;
        ~TrunkControllerDebug();

		operator TrunkControllerDebugMsg() const override;
		TrunkControllerDebug& operator= (const TrunkControllerDebugMsg&) override;

		robotlib::LegDataMap<Eigen::Vector3d> feet_forces;
		robotlib::LegDataMap<Eigen::Vector3d> desired_forces;
		Eigen::Matrix<double,6,1> desired_wrench;
	};
} // end namespace dls

#endif /* end of include guard: TRUNK_CONTROLLER_DEBUG_HPP */
