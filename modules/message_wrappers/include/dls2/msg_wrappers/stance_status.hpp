
#ifndef STANCE_STATUS_HPP
#define STANCE_STATUS_HPP

#include "dls2/msg_wrappers/wrapper.hpp"
#include "dls_messages/dds/stance_status.hpp"
#include "robotlib/robot_base.hpp"

namespace dls
{
	class StanceStatus : public Wrapper<StanceStatusMsg>
	{
	public:
		template <typename SignalType>
		friend class SignalWriter;
		friend class Plugin;

		StanceStatus(const std::shared_ptr<robotlib::RobotBase> robot);
		StanceStatus(StanceStatus &stance_status);
		StanceStatus() = delete;
		virtual ~StanceStatus();

		operator StanceStatusMsg() const override;
		StanceStatus &operator=(const StanceStatusMsg &stance_status_msg) override;
		StanceStatus &operator=(const StanceStatus &stance_status);

		std::string frame_id_{};
		uint32_t sequence_id_{};
		double timestamp_{};

		robotlib::LegDataMap<bool> stance_status_;

	private:
		virtual void setDataFromWrapperBase(WrapperBase *wrapper_base) override;
	};
} // namespace dls

#endif