
#ifndef TRUNK_CONTROLLER_DEBUG_HPP
#define TRUNK_CONTROLLER_DEBUG_HPP

#include "dls2/msg_wrappers/wrapper.hpp"
#include "dls_messages/dds/trunk_controller_debug.h"
#include "robotlib/robot_base.hpp"

namespace dls
{
	class TrunkControllerDebug : public Wrapper<TrunkControllerDebugMsg>
	{
	public:
		TrunkControllerDebug(const std::shared_ptr<robotlib::RobotBase> robot);
		TrunkControllerDebug(TrunkControllerDebug& trunk_controller_debug);
		TrunkControllerDebug() = delete;
        virtual ~TrunkControllerDebug();

		operator TrunkControllerDebugMsg() const override;
		TrunkControllerDebug& operator=(const TrunkControllerDebugMsg& trunk_controller_debug_msg) override;
		TrunkControllerDebug& operator=(const TrunkControllerDebug& trunk_controller_debug);

		std::string frame_id_{};
		uint32_t sequence_id_{};
		double timestamp_{};

		robotlib::LegDataMap<Eigen::Vector3d> feet_forces_;
		robotlib::LegDataMap<Eigen::Vector3d> desired_forces_;
		Eigen::Matrix<double,6,1> desired_wrench_{};
	};
} // namespace dls

#endif