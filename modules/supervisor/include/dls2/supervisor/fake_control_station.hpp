
#pragma once

#include <string>
#include <vector>
#include <chrono>
#include <list>
#include <thread>
#include <queue>
#include <cstdlib>
#include <mutex>
#include <type_traits>
#include <array>

#include "dls2/topics/topics.hpp"
#include <dls_messages/dds/CSBasicCommand.hpp>
#include <dls_messages/dds/CSEmergency.hpp>
#include <dls_messages/dds/TargetPosition.hpp>
#include <dls_messages/dds/StayOutZones.hpp>
#include <dls_messages/dds/ReferencePath.hpp>
#include <dls_messages/dds/FeetReference.hpp>
#include <dls_messages/dds/BaseReference.hpp>
#include <dls_messages/dds/JointStates.hpp>
#include <dls_messages/dds/BaseReference.hpp>
#include <dls_messages/dds/CommandCall.hpp>
#include <dls_messages/dds/DlsStatus.hpp>
#include <dls_messages/dds/DlsEvents.hpp>
#include <dls_messages/dds/DesiredTorques.hpp>

#include "dls2/state_machine/state_machine.hpp"
#include "dls2/supervisor/orchestrator_base.hpp"
#include "dls2/util/time/duration_utils.hpp"

#include "dls2/supervisor/pegasus_orchestrator.hpp"

#include "dls2/plugin/periodic_app_plugin.hpp"
#include "dls2/log/log.hpp"

namespace dls
{

	class FakeControlStation : public PeriodicAppPlugin {

	public:

		explicit FakeControlStation(const std::string &ID);
		~FakeControlStation() = default;

    	void run(const std::chrono::system_clock::time_point &time) override;
	
    private:
        
        void inputThread();
        
        void send_emergency_on_command();
        void send_emergency_off_command();
        void send_target_position_command();
        void send_stay_out_zones_command();
        void send_reference_path_command();
        void send_feet_reference_command();
        void send_base_reference_command();
        void send_joint_states_command();
        void send_cs_loc_reset_command();
        void send_command_call_command();

        void autonomy_level_manual();
        void autonomy_level_autonomous();
        void locomotion_strategy_mpc();
        void locomotion_strategy_rl();
        void control_strategy_target_position();
        void control_strategy_reference_path();
        void control_strategy_stay_out_zones();
        void control_strategy_feet_reference();
        void control_strategy_base_reference();
        void control_strategy_joint_reference();
        void control_strategy_loc_reset();

        WriterPtr<dls2_interface::msg::CSBasicCommand> cs_basic_command_writer;
        WriterPtr<dls2_interface::msg::CSEmergency> cs_emergency_writer;
        WriterPtr<dls2_interface::msg::TargetPosition> target_position_writer;
        WriterPtr<dls2_interface::msg::StayOutZones> stay_out_zones_writer;
        WriterPtr<dls2_interface::msg::ReferencePath> reference_path_writer;
        WriterPtr<dls2_interface::msg::FeetReference> feet_reference_writer;
        WriterPtr<dls2_interface::msg::BaseReference> base_reference_writer;
        WriterPtr<dls2_interface::msg::JointStates> joint_states_writer;
        WriterPtr<dls2_interface::msg::BaseReference> cs_loc_reset_writer;
        WriterPtr<dls2_interface::msg::CommandCall> command_call_writer;

        AutonomyLevel autonomy_level_ {AutonomyLevel::AUTONOMOUS};
        LocomotionStrategy locomotion_strategy_{LocomotionStrategy::MPC};
        ControlStrategy control_strategy_{ControlStrategy::TARGET_POSITION};

        std::queue<int> commands;
        std::mutex mtx;
    };

} // end namespace dls