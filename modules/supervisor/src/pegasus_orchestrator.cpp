

#include "dls2/supervisor/pegasus_orchestrator.hpp"

namespace dls
{
    PegasusOrchestrator::PegasusOrchestrator(const std::string &ID, const double& cs_timeout_sec, const double& cs_manual_timeout_sec, size_t expected_joint_size)
        : dls::OrchestratorBase(ID)
        , cs_standard_timeout_sec_(cs_timeout_sec)
        , cs_manual_timeout_sec_(cs_manual_timeout_sec)
        , expected_joint_size_(expected_joint_size)
    {

        // Telemetry-related readers and writers definition
        ReaderPtr<dls2_interface::msg::CSBasicCommand> cs_basic_command_reader = std::make_shared<dls::Reader<dls2_interface::msg::CSBasicCommand>>(this->dds_participant_, dls::topics::cs_basic_command);
        ReaderPtr<dls2_interface::msg::CSEmergency> cs_emergency_reader = std::make_shared<dls::Reader<dls2_interface::msg::CSEmergency>>(this->dds_participant_, dls::topics::cs_emergency);
        ReaderPtr<dls2_interface::msg::TargetPosition> target_position_reader = std::make_shared<dls::Reader<dls2_interface::msg::TargetPosition>>(this->dds_participant_, dls::topics::cs_target_position);
        ReaderPtr<dls2_interface::msg::StayOutZones> stay_out_zones_reader = std::make_shared<dls::Reader<dls2_interface::msg::StayOutZones>>(this->dds_participant_, dls::topics::cs_stay_out_zones);
        ReaderPtr<dls2_interface::msg::ReferencePath> reference_path_reader = std::make_shared<dls::Reader<dls2_interface::msg::ReferencePath>>(this->dds_participant_, dls::topics::cs_reference_path);
        ReaderPtr<dls2_interface::msg::FeetReference> feet_reference_reader = std::make_shared<dls::Reader<dls2_interface::msg::FeetReference>>(this->dds_participant_, dls::topics::cs_feet_reference);
        ReaderPtr<dls2_interface::msg::BaseReference> base_reference_reader = std::make_shared<dls::Reader<dls2_interface::msg::BaseReference>>(this->dds_participant_, dls::topics::cs_base_reference);
        ReaderPtr<dls2_interface::msg::JointStates> joint_states_reader = std::make_shared<dls::Reader<dls2_interface::msg::JointStates>>(this->dds_participant_, dls::topics::cs_joint_states);
        ReaderPtr<dls2_interface::msg::BaseReference> cs_loc_reset_reader = std::make_shared<dls::Reader<dls2_interface::msg::BaseReference>>(this->dds_participant_, dls::topics::cs_loc_reset);
        ReaderPtr<dls2_interface::msg::CommandCall> command_call_reader = std::make_shared<dls::Reader<dls2_interface::msg::CommandCall>>(this->dds_participant_, dls::topics::cs_command_call);

        WriterPtr<dls2_interface::msg::DlsStatus> dls_status_writer = std::make_shared<dls::Writer<dls2_interface::msg::DlsStatus>>(this->dds_participant_,dls::topics::dls_status);
        WriterPtr<dls2_interface::msg::DlsEvents> dls_events_writer = std::make_shared<dls::Writer<dls2_interface::msg::DlsEvents>>(this->dds_participant_,dls::topics::dls_events);

        // Bind readers to PegasusInput fields 
        this->telemetry_manager_.bindReader<dls2_interface::msg::CSBasicCommand>(cs_basic_command_reader, &PegasusInput::cs_basic_command_msg);
        this->telemetry_manager_.bindReader<dls2_interface::msg::CSEmergency>(cs_emergency_reader, &PegasusInput::cs_emergency_msg);
        this->telemetry_manager_.bindReader<dls2_interface::msg::TargetPosition>(target_position_reader, &PegasusInput::target_position_msg);
        this->telemetry_manager_.bindReader<dls2_interface::msg::StayOutZones>(stay_out_zones_reader, &PegasusInput::stay_out_zones_msg);
        this->telemetry_manager_.bindReader<dls2_interface::msg::ReferencePath>(reference_path_reader, &PegasusInput::reference_path_msg);
        this->telemetry_manager_.bindReader<dls2_interface::msg::FeetReference>(feet_reference_reader, &PegasusInput::feet_reference_msg);
        this->telemetry_manager_.bindReader<dls2_interface::msg::BaseReference>(base_reference_reader, &PegasusInput::base_reference_msg);
        this->telemetry_manager_.bindReader<dls2_interface::msg::JointStates>(joint_states_reader, &PegasusInput::joint_states_msg);
        this->telemetry_manager_.bindReader<dls2_interface::msg::BaseReference>(base_reference_reader, &PegasusInput::loc_reset_msg);
        this->telemetry_manager_.bindReader<dls2_interface::msg::CommandCall>(command_call_reader, &PegasusInput::command_call_msg);

        // Bind writers to PegasusOutput fields
        this->telemetry_manager_.bindWriter<dls2_interface::msg::DlsStatus>(dls_status_writer,
                                         &PegasusOutput::dls_status_msg);

        this->telemetry_manager_.bindWriter<dls2_interface::msg::DlsEvents>(dls_events_writer,
                                         &PegasusOutput::dls_events_msg);
    }

    void PegasusOrchestrator::goToStatus(OrchestratorStatus new_status){
        if(status_ != new_status){
            prev_status_ = status_;
        }else{
            return;
        }
        status_ = new_status;
    }

    void PegasusOrchestrator::goToPrevStatus(){
        status_ = prev_status_;
    }

    bool PegasusOrchestrator::allowed(const AutonomyLevel& autonomy_level, 
                                      const ControlStrategy& control_strategy){
        if(autonomy_level == AutonomyLevel::AUTONOMOUS)
        {
            return 
                control_strategy == ControlStrategy::TARGET_POSITION || 
                control_strategy == ControlStrategy::REFERENCE_PATH;
        }else if(autonomy_level == AutonomyLevel::MANUAL)
        {
            return 
                control_strategy == ControlStrategy::FEET_REFERENCE || 
                control_strategy == ControlStrategy::BASE_REFERENCE || 
                control_strategy == ControlStrategy::JOINT_REFERENCE;
        }
        return false;
    }

    bool PegasusOrchestrator::allowed(const ControlStrategy& control_strategy, 
                                      const LocomotionStrategy& locomotion_strategy){
        if(control_strategy == ControlStrategy::FEET_REFERENCE || 
           control_strategy == ControlStrategy::JOINT_REFERENCE)
        {
            return locomotion_strategy == LocomotionStrategy::MPC;
        }
        return true;
    }

    void PegasusOrchestrator::orchestrate(const std::chrono::system_clock::time_point &time, 
                                          const EventsPriorityQueue &events){

        std::cout << "status: " << OrchestratorStatusTypes[to_underlying(status_)];
        std::cout << "\nautonomy_level: " << OrchestratorAutonomyLevelTypes[to_underlying(autonomy_level_)] <<
        "; control_strategy: " << OrchestratorControlStrategyTypes[to_underlying(control_strategy_)] <<
        "; locomotion_strategy: " << OrchestratorLocomotionStrategyTypes[to_underlying(locomotion_strategy_)] << "\n";

        if(status_ == OrchestratorStatus::INITIALIZATION){
            // TODO: perform some checks
            goToStatus(OrchestratorStatus::WAITING_FOR_REFERENCE);
        }

        // 0. Checking triggered events, if at least one err stop
        bool err_found = !events.empty() && 
            (events.top().severity() == to_underlying(EventSeverity::ERROR) || 
             events.top().severity() == to_underlying(EventSeverity::FATAL));
        is_event_queue_ok_ = !err_found;

        if(err_found){
            // TODO: trigger stop, go to state machine update directly
            std::cout << "orch: err/fatal event found!\n";
            goToStatus(OrchestratorStatus::STOP);
        }

        // Consuming latest control station alive and commands
        {
            std::lock_guard<std::mutex> input_lock(dls_input_.mutex);
            // 1. CSBasicCommand: check if msg is too old
            // auto basic_command_delta_sec = toSec<double>(time - fromNs(dls_input_.cs_basic_command_msg.header().timestamp()));

            auto cs_time = fromNs<std::chrono::system_clock::time_point>(dls_input_.cs_basic_command_msg.header().timestamp());
            auto basic_command_delta_sec = toSec<double>(time - cs_time);

            is_cs_basic_command_ok_ = basic_command_delta_sec < cs_standard_timeout_sec_ && dls_input_.cs_basic_command_msg.enable();
            if(is_cs_basic_command_ok_){
                // std::cout << "orch: cs_basic_command received\n";
            }else{
                // TODO: trigger stop and stop consuming control station msgs, go to state machine update directly
                goToStatus(OrchestratorStatus::STOP);
            }

            const auto& autonomy_level = dls_input_.cs_basic_command_msg.autonomy_level();
            const auto& locomotion_strategy = dls_input_.cs_basic_command_msg.locomotion_strategy();
            const auto& control_strategy = dls_input_.cs_basic_command_msg.control_strategy();

            // 2. Check if stop requested (dls_input_.cs_emergency_msg)
            auto emergency_delta_sec = toSec<double>(time - fromNs(dls_input_.cs_emergency_msg.header().timestamp()));
            auto emergency_triggered = emergency_delta_sec < cs_standard_timeout_sec_ && dls_input_.cs_emergency_msg.emergency_stop() == true;
            if(emergency_triggered){
                has_emergency_been_triggered_ = true;
            }

            is_emergency_stop_msg_ok_ = true;
            if(has_emergency_been_triggered_ && (emergency_delta_sec > cs_standard_timeout_sec_ || dls_input_.cs_emergency_msg.emergency_stop() == true)){
                std::cout << "emergency_delta_sec > cs_standard_timeout_sec_: " << (emergency_delta_sec > cs_standard_timeout_sec_) << "; emergency_stop: " << dls_input_.cs_emergency_msg.emergency_stop() << "\n";
                is_emergency_stop_msg_ok_ = false;
            }

            if(emergency_triggered){ // Execute it if sent recently
                // TODO: trigger stop / hard stop?
                goToStatus(OrchestratorStatus::STOP);
                std::cout << "orch: cs_emergency_msg received\n";
            }

            // 3. Check if any console command has been sent from cs (dls_input_.command_call_msg)
            auto command_call_delta_sec = toSec<double>(time - fromNs(dls_input_.command_call_msg.header().timestamp()));
            if(command_call_delta_sec < cs_standard_timeout_sec_){ // Execute it if sent recently
                // TODO: parse and execute console command
            }

            // 4. Check if stay_out_zones_msg recently arrived
            auto stay_out_zones_delta_sec = toSec<double>(time - fromNs(dls_input_.stay_out_zones_msg.header().timestamp()));
            if(stay_out_zones_delta_sec < cs_standard_timeout_sec_){
                // TODO: forward stay-out zones to planner
                std::cout << "stay_out_zones_msg received\n";
            }

            // 5. Check if basic command has been updated during execution
            
            if(status_ == OrchestratorStatus::EXECUTING_REFERENCE || status_ == OrchestratorStatus::WAITING_FOR_REFERENCE){

                // 5.1 Check if autonomy level has been updated
                if(autonomy_level != to_underlying(autonomy_level_))
                {
                    std::optional<AutonomyLevel> autonomy_level_enum_opt = to_enum_checked<AutonomyLevel>(autonomy_level);
                    if(autonomy_level_enum_opt.has_value()){
                        std::cout << "autonomy_level update\n";
                        autonomy_level_ = autonomy_level_enum_opt.value();
                        goToStatus(OrchestratorStatus::WAITING_FOR_REFERENCE);
                    }
                }

                // 5.2 Check if control strategy has been updated
                if(control_strategy != to_underlying(control_strategy_))
                {
                    auto control_strategy_enum_opt = to_enum_checked<ControlStrategy>(control_strategy);
                    if(control_strategy_enum_opt.has_value() && 
                        allowed(autonomy_level_, control_strategy_enum_opt.value()))
                    {
                        std::cout << "control strategy update\n";
                        control_strategy_ = control_strategy_enum_opt.value();
                        goToStatus(OrchestratorStatus::WAITING_FOR_REFERENCE);
                    }else{
                        std::cout << "Not valid control strategy, discarded\n";
                    }
                }

                // 5.3 Check if locomotion strategy has been updated
                if(locomotion_strategy != to_underlying(locomotion_strategy_))
                {
                    // TODO: request locomotion strategy update, go to state machine update directly
                    std::optional<LocomotionStrategy> locomotion_strategy_enum_opt = to_enum_checked<LocomotionStrategy>(locomotion_strategy);
                    if(locomotion_strategy_enum_opt.has_value() && 
                        allowed(control_strategy_, locomotion_strategy_enum_opt.value())){
                        std::cout << "locomotion_strategy update\n";
                        locomotion_strategy_ = locomotion_strategy_enum_opt.value();
                        goToStatus(OrchestratorStatus::SWITCHING_CONTROLLER);
                    }else{
                        std::cout << "Not valid locomotion strategy, discarded\n";
                    }
                }

                // 6. Check if any control msg has been sent from cs
                if(autonomy_level_ == AutonomyLevel::AUTONOMOUS){
                    switch (control_strategy_)
                    {
                        case ControlStrategy::TARGET_POSITION:
                        {
                            // Check if dls_input_.target_position_msg recently arrived
                            auto target_position_delta_sec = toSec<double>(time - fromNs(dls_input_.target_position_msg.header().timestamp()));
                            if(target_position_delta_sec < cs_standard_timeout_sec_){
                                // TODO: plan a path to desired point
                                std::cout << "orch: target_position_msg received\n";
                                std::this_thread::sleep_for(std::chrono::milliseconds(500));
                                goToStatus(OrchestratorStatus::EXECUTING_REFERENCE);
                            }
                            break;

                        }
                        case ControlStrategy::REFERENCE_PATH:
                        {
                            // Check if dls_input_.reference_path_msg recently arrived
                            auto reference_path_delta_sec = toSec<double>(time - fromNs(dls_input_.reference_path_msg.header().timestamp()));
                            if(reference_path_delta_sec < cs_standard_timeout_sec_){
                                // TODO: plan a path to desired point
                                std::cout << "orch: reference_path_msg received\n";
                                goToStatus(OrchestratorStatus::EXECUTING_REFERENCE);
                            }
                            break;
                        }

                        default:
                            std::cout << "Unexpected control_strategy_ value = " << to_underlying(control_strategy_) << " with autonomy_level_ " << to_underlying(autonomy_level_) << "\n";
                            break;
                    }

                }else if(autonomy_level_ == AutonomyLevel::MANUAL){
                    switch (control_strategy_)
                    {
                        case ControlStrategy::FEET_REFERENCE:
                        {
                            // Check if dls_input_.feet_reference_msg recently arrived
                            auto feet_reference_delta_sec = toSec<double>(time - fromNs(dls_input_.feet_reference_msg.header().timestamp()));
                            if(feet_reference_delta_sec < cs_manual_timeout_sec_){
                                // TODO: forward/set feet reference to/for controller
                                goToStatus(OrchestratorStatus::EXECUTING_REFERENCE); 
                            }else{
                                std::cout << "ASYNC msg not arrived in time!\n";
                                goToStatus(OrchestratorStatus::WAITING_FOR_REFERENCE); 
                            }
                            break;

                        }
                        case ControlStrategy::BASE_REFERENCE:
                        {
                            // Check if dls_input_.base_reference_msg recently arrived
                            auto base_reference_delta_sec = toSec<double>(time - fromNs(dls_input_.base_reference_msg.header().timestamp()));
                            if(base_reference_delta_sec < cs_manual_timeout_sec_){
                                std::cout << "orch: base_reference_msg received\n";
                                // TODO: forward/set base reference to/for controller
                                goToStatus(OrchestratorStatus::EXECUTING_REFERENCE); 
                            }
                            break;

                        }
                        case ControlStrategy::JOINT_REFERENCE:
                        {
                            // Check if dls_input_.joint_states_msg recently arrived
                            auto joint_states_delta_sec = toSec<double>(time - fromNs(dls_input_.joint_states_msg.header().timestamp()));
                            if(joint_states_delta_sec < cs_manual_timeout_sec_){
                                if(dls_input_.joint_states_msg.joints().position().size() == expected_joint_size_ || 
                                 dls_input_.joint_states_msg.joints().velocity().size() == expected_joint_size_ || 
                                 dls_input_.joint_states_msg.joints().effort().size() == expected_joint_size_)
                                 {
                                    // TODO: forward/set joint reference to/for controller
                                    goToStatus(OrchestratorStatus::EXECUTING_REFERENCE); 
                                 }else
                                 {
                                    std::cout << "Provided message size not matching expected size, discarded\n";
                                 }
                            }
                            break;

                        }
                        case ControlStrategy::LOC_RESET:
                        {
                            // Check if dls_input_.base_reference_msg recently arrived
                            auto loc_reset_delta_sec = toSec<double>(time - fromNs(dls_input_.loc_reset_msg.header().timestamp()));
                            if(loc_reset_delta_sec < cs_standard_timeout_sec_){
                                // TODO: set localization state
                                goToStatus(OrchestratorStatus::EXECUTING_REFERENCE);
                            }
                            break;
                        }

                        default:
                            std::cout << "Unexpected control_strategy value = " << to_underlying(control_strategy_) << " with autonomy_level_ " << to_underlying(autonomy_level_) << "\n";
                            break;
                    }
                }
            }
        
            // TODO: if in SWITCHING_CONTROLLER or in STOP state, wait for updates
            if(status_ == OrchestratorStatus::SWITCHING_CONTROLLER){
                // TODO: wait for completion
                std::cout << "switching scope\n";
                // goToPrevStatus();
            }
            if(status_ == OrchestratorStatus::STOP){
                // TODO: wait for solved issue
                std::cout << "Robot stopped (basic cs command ok: " << is_cs_basic_command_ok_ << ", emergency ok: " 
                    << is_emergency_stop_msg_ok_ <<", event ok: " << is_event_queue_ok_ << ")\n";
                if(is_cs_basic_command_ok_ && is_emergency_stop_msg_ok_ && is_event_queue_ok_){
                    has_emergency_been_triggered_ = false;
                    goToPrevStatus();
                }
            }
        
            std::lock_guard<std::mutex> output_lock(dls_output_.mutex);
            // TODO: fill in dls_output_.dls_status_msg
        }
    }

    void PegasusOrchestrator::telemetryMain(const std::vector<dls2_interface::msg::EventLog> &events_to_publish)
    {
         {
            std::lock_guard<std::mutex> lock(dls_output_.mutex);
            dls_output_.dls_events_msg.events() = events_to_publish;    
        }
        this->telemetry_manager_.tick(dls_input_, dls_output_);
    }

}
