
#pragma once

#include <string>
#include <vector>
#include <list>
#include <mutex>
#include <type_traits>
#include <magic_enum/magic_enum.hpp>
#include <optional>

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


namespace dls
{

	template <typename E>
	constexpr std::underlying_type_t<E> to_underlying(E e) noexcept {
		return static_cast<std::underlying_type_t<E>>(e);
	}

	template <class E, class I>
	std::optional<E> to_enum_checked(I v) {
		return magic_enum::enum_cast<E>(v); // empty optional if not a valid enumerator
	}

	enum class OrchestratorStatus : uint8_t {
		INITIALIZATION = 0,
		WAITING_FOR_REFERENCE,
		EXECUTING_REFERENCE,
		STOP,
		SWITCHING_CONTROLLER
	};

	constexpr std::string_view OrchestratorStatusTypes[] =
	{
		"INITIALIZATION",
		"WAITING_FOR_REFERENCE",
		"EXECUTING_REFERENCE",
		"STOP",
		"SWITCHING_CONTROLLER"
	};

	enum class AutonomyLevel : uint8_t {
		AUTONOMOUS = 0,
		MANUAL
	};

	constexpr std::string_view OrchestratorAutonomyLevelTypes[] =
	{
		"AUTONOMOUS",
		"MANUAL"
	};

	enum class ControlStrategy : uint8_t {
		TARGET_POSITION = 0,
		STAY_OUT_ZONES,
		REFERENCE_PATH,
		FEET_REFERENCE,
		BASE_REFERENCE,
		JOINT_REFERENCE,
		LOC_RESET
	};

	constexpr std::string_view OrchestratorControlStrategyTypes[] =
	{
		"TARGET_POSITION",
		"REFERENCE_PATH",
		"STAY_OUT_ZONES",
		"FEET_REFERENCE",
		"BASE_REFERENCE",
		"JOINT_REFERENCE",
		"LOC_RESET"
	};

	enum class LocomotionStrategy : uint8_t {
		MPC = 0,
		RL,
		RL_3_LEGS
	};

	constexpr std::string_view OrchestratorLocomotionStrategyTypes[] =
	{
		"MPC",
		"RL",
		"RL_3_LEGS"
	};
	
	struct PegasusInput 
	{
		std::mutex mutex;
		dls2_interface::msg::CSBasicCommand cs_basic_command_msg;
		dls2_interface::msg::CSEmergency cs_emergency_msg;
		dls2_interface::msg::TargetPosition target_position_msg;
		dls2_interface::msg::StayOutZones stay_out_zones_msg;
		dls2_interface::msg::ReferencePath reference_path_msg;
		dls2_interface::msg::FeetReference feet_reference_msg;
		dls2_interface::msg::BaseReference base_reference_msg;
		dls2_interface::msg::JointStates joint_states_msg;
		dls2_interface::msg::BaseReference loc_reset_msg;
		dls2_interface::msg::CommandCall command_call_msg;
	};

	struct PegasusOutput 
	{
		std::mutex mutex;
		dls2_interface::msg::DlsStatus dls_status_msg;
		dls2_interface::msg::DlsEvents dls_events_msg;
	};

	struct PegasusInternalFeedback {
		std::mutex mutex;
		dls2_interface::msg::DesiredTorques desired_torques_msg; // TODO: could be readers directly (no mutex needed)
		// TODO: add many others...
	};

	struct PegasusInternalAction {
		std::mutex mutex;
		// dls2_interface::msg::PlannerGoal planner_goal_msg; // TODO: could be writers directly (no mutex needed)
		// TODO: add many others...
	};


	class PegasusOrchestrator : public OrchestratorBase
	{

	public:
		PegasusOrchestrator(const std::string &ID, const std::shared_ptr<state_machine::StateMachine> &sm = nullptr);

		void orchestrate(const std::chrono::system_clock::time_point &time, const EventsPriorityQueue &events) override;
        void telemetryMain(const std::vector<dls2_interface::msg::EventLog> &events_to_publish) override;

	private:

		void goToStatus(OrchestratorStatus new_status);
		void goToPrevStatus();

		PegasusInput dls_input_;
		PegasusOutput dls_output_;

		PegasusInternalFeedback dls_internal_feedback_;
		PegasusInternalAction dls_internal_action_;

		double cs_timeout_sec_{ 1 };

		AutonomyLevel autonomy_level_{AutonomyLevel::AUTONOMOUS};
		ControlStrategy control_strategy_{ControlStrategy::TARGET_POSITION};
		LocomotionStrategy locomotion_strategy_{LocomotionStrategy::MPC};

		OrchestratorStatus status_{OrchestratorStatus::INITIALIZATION};
		OrchestratorStatus prev_status_{OrchestratorStatus::INITIALIZATION};

		bool has_emergency_been_triggered_ {false};
		bool is_emergency_stop_msg_ok_ {true};
		bool is_cs_basic_command_ok_ {true};
		bool is_event_queue_ok_ {true};

	};
} // end namespace dls