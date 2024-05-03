
#ifndef MPC_GENERATOR_OUTPUT_HPP
#define MPC_GENERATOR_OUTPUT_HPP

#include "dls2/msg_wrappers/wrapper.hpp"
#include "dls_messages/dds/mpc_generator_output.h"
#include "dls2/msg_wrappers/pose.hpp"
#include "dls2/msg_wrappers/screw.hpp"
#include "robotlib/robot_base.hpp"

namespace dls
{
	class MPCGeneratorOutput : public Wrapper<MPCGeneratorOutputMsg>
	{
	public:
	    template <typename SignalType>
		friend class SignalWriter;
		friend class Plugin;

		MPCGeneratorOutput(const std::shared_ptr<robotlib::RobotBase> robot);
		MPCGeneratorOutput(MPCGeneratorOutput& mpc_generator_output);
		MPCGeneratorOutput() = delete;
		virtual ~MPCGeneratorOutput();

		operator MPCGeneratorOutputMsg() const override;
		MPCGeneratorOutput& operator=(const MPCGeneratorOutputMsg& mpc_generator_output_msg) override;
		MPCGeneratorOutput& operator=(const MPCGeneratorOutput& mpc_generator_output);

        std::string frame_id_{};
        uint32_t sequence_id_{};
        double timestamp_{};

        std::vector<dls::Pose> desired_base_poses_{};
        std::vector<dls::Screw> desired_base_velocities_{};
        std::vector<dls::Screw> desired_base_accelerations_{};

        std::vector<robotlib::JointState> desired_joints_positions_;
        std::vector<robotlib::JointState> desired_joints_velocities_;
        std::vector<robotlib::JointState> desired_joints_accelerations_;
        std::vector<robotlib::JointState> desired_torques_;

        std::vector<robotlib::LegDataMap<Eigen::Vector3d>> desired_feet_positions_;
        std::vector<robotlib::LegDataMap<Eigen::Vector3d>> desired_feet_velocities_;
        std::vector<robotlib::LegDataMap<Eigen::Vector3d>> desired_feet_accelerations_;
        std::vector<robotlib::LegDataMap<Eigen::Vector3d>> desired_ground_reaction_forces_;

        std::vector<robotlib::LegDataMap<bool>> desired_contacts_sequence_;
    private:
        virtual void setDataFromWrapperBase(WrapperBase *wrapper_base) override;
	};
} // namespace dls

#endif