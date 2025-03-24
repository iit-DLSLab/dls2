
#ifndef TRUNK_CONTROLLER_DEBUG_HPP
#define TRUNK_CONTROLLER_DEBUG_HPP

#include "dls2/signal/wrapper.hpp"
#include "dls_messages/dds/trunk_controller_debug.hpp"
#include "robotlib/robot_base.hpp"

namespace dls
{
	class TrunkControllerDebug : public Wrapper<TrunkControllerDebugMsg>
	{
	public:
        template <typename SignalType>
        friend class SignalWriter;
        friend class Plugin;

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

		robotlib::LimbDataMap<Eigen::Vector3d> feet_forces_;
		robotlib::LimbDataMap<Eigen::Vector3d> desired_forces_;
		Eigen::Matrix<double,6,1> desired_wrench_{};
		robotlib::LimbDataMap<double> friction_coefficients_;
		robotlib::JointState gravity_term_;
		robotlib::JointState coriolis_centrifugal_terms_;
    private:
        virtual void setDataFromWrapperBase(WrapperBase *wrapper_base) override;
	};
} // namespace dls

#endif