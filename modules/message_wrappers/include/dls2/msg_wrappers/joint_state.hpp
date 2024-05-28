#ifndef JOINT_STATE_HPP
#define JOINT_STATE_HPP

#include "dls2/msg_wrappers/wrapper.hpp"
#include "dls_messages/dds/joint_state.h"
#include "robotlib/robot_base.hpp"

#include <Eigen/Dense>

namespace dls
{
    class JointState : public Wrapper<JointStateMsg>
    {
    public:
        template <typename SignalType>
        friend class SignalWriter;
        friend class Plugin;
        JointState(const std::shared_ptr<robotlib::RobotBase> robot);
        JointState(const JointState &JointState);
        virtual ~JointState();

        operator JointStateMsg() const override;
        JointState &operator=(const JointStateMsg &JointState_msg) override;
        JointState &operator=(const JointState &JointState);

        std::string frame_id_{};
        uint32_t sequence_id_{};
        double timestamp_{};

        robotlib::JointState q_;
        robotlib::JointState qd_;
        robotlib::JointState qdd_;

    private:
        virtual void setDataFromWrapperBase(WrapperBase *wrapper_base) override;
    };
} // namespace dls

#endif