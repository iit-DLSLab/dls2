

#include "dls2/supervisor/pegasus_orchestrator.hpp"

namespace dls
{
    PegasusOrchestrator::PegasusOrchestrator(const std::string &ID, const std::shared_ptr<state_machine::StateMachine> &sm)
        : dls::OrchestratorBase(ID, sm)
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

    void PegasusOrchestrator::orchestrate(const std::chrono::system_clock::time_point &time, 
                                          const EventsPriorityQueue &events){

        // TODO: check events -> how to react?

        // Consuming latest control station alive and commands    
        {
            std::lock_guard<std::mutex> input_lock(dls_input_.mutex);
            // 1. CSBasicCommand: check if msg is too old
            auto basic_command_delta_sec = toSec<double>(time - fromNs(dls_input_.cs_basic_command_msg.header().timestamp()));
            if(!dls_input_.cs_basic_command_msg.enable() || basic_command_delta_sec > cs_timeout_sec_){
                // TODO: trigger stop and stop consuming control station msgs, go to state machine update directly
            }

            const auto& autonomy_level = dls_input_.cs_basic_command_msg.autonomy_level();
            const auto& locomotion_strategy = dls_input_.cs_basic_command_msg.locomotion_strategy();
            const auto& control_strategy = dls_input_.cs_basic_command_msg.control_strategy();

            // 2. Check if stop requested (dls_input_.cs_emergency_msg)
            auto emergency_delta_sec = toSec<double>(time - fromNs(dls_input_.cs_emergency_msg.header().timestamp()));
            if(emergency_delta_sec < cs_timeout_sec_){ // Execute it if sent recently
                // TODO: trigger stop / hard stop?
            }

            // 3. Check if any console command has been sent from cs (dls_input_.command_call_msg)
            auto command_call_delta_sec = toSec<double>(time - fromNs(dls_input_.command_call_msg.header().timestamp()));
            if(command_call_delta_sec < cs_timeout_sec_){ // Execute it if sent recently
                // TODO: parse and execute console command
            }

            // 4. Check if locomotion strategy update has been triggered
            if(locomotion_strategy_ != locomotion_strategy){
                // TODO: request locomotion strategy update
            }
            
            // 5. Check if any control msg has been sent from cs
            if(autonomy_level == static_cast<uint8_t>(AutonomyLevel::AUTONOMOUS)){
                switch (control_strategy)
                {
                    case static_cast<uint8_t>(ControlStrategy::TARGET_POSITION):
                    {
                        // Check if dls_input_.target_position_msg recently arrived
                        auto target_position_delta_sec = toSec<double>(time - fromNs(dls_input_.target_position_msg.header().timestamp()));
                        if(target_position_delta_sec < cs_timeout_sec_){
                            // TODO: plan a path to desired point
                        }
                        break;

                    }
                    case static_cast<uint8_t>(ControlStrategy::REFERENCE_PATH):
                    {
                        // Check if dls_input_.reference_path_msg recently arrived
                        auto reference_path_delta_sec = toSec<double>(time - fromNs(dls_input_.reference_path_msg.header().timestamp()));
                        if(reference_path_delta_sec < cs_timeout_sec_){
                            // TODO: plan a path to desired point
                        }
                        break;

                    }
                    case static_cast<uint8_t>(ControlStrategy::STAY_OUT_ZONES):
                    {
                        // Check if dls_input_.stay_out_zones_msg recently arrived
                        auto stay_out_zones_delta_sec = toSec<double>(time - fromNs(dls_input_.stay_out_zones_msg.header().timestamp()));
                        if(stay_out_zones_delta_sec < cs_timeout_sec_){
                            // TODO: forward stay-out zones to planner
                        }
                        break;
                    }

                    default:
                        std::cout << "Unexpected control_strategy value = " << control_strategy << " with autonomy_level " << autonomy_level << "\n";
                        break;
                }

            }else if(autonomy_level == static_cast<uint8_t>(AutonomyLevel::MANUAL)){
                switch (control_strategy)
                {
                    case static_cast<uint8_t>(ControlStrategy::FEET_REFERENCE):
                    {
                        // Check if dls_input_.feet_reference_msg recently arrived
                        auto feet_reference_delta_sec = toSec<double>(time - fromNs(dls_input_.feet_reference_msg.header().timestamp()));
                        if(feet_reference_delta_sec < cs_timeout_sec_){
                            // TODO: forward/set feet reference to/for controller
                        }
                        break;

                    }
                    case static_cast<uint8_t>(ControlStrategy::BASE_REFERENCE):
                    {
                        // Check if dls_input_.base_reference_msg recently arrived
                        auto base_reference_delta_sec = toSec<double>(time - fromNs(dls_input_.base_reference_msg.header().timestamp()));
                        if(base_reference_delta_sec < cs_timeout_sec_){
                            // TODO: forward/set base reference to/for controller
                        }
                        break;

                    }
                    case static_cast<uint8_t>(ControlStrategy::JOINT_REFERENCE):
                    {
                        // Check if dls_input_.joint_states_msg recently arrived
                        auto joint_states_delta_sec = toSec<double>(time - fromNs(dls_input_.joint_states_msg.header().timestamp()));
                        if(joint_states_delta_sec < cs_timeout_sec_){
                            // TODO: forward/set joint reference to/for controller
                        }
                        break;

                    }
                    case static_cast<uint8_t>(ControlStrategy::LOC_RESET):
                    {
                        // Check if dls_input_.base_reference_msg recently arrived
                        auto loc_reset_delta_sec = toSec<double>(time - fromNs(dls_input_.loc_reset_msg.header().timestamp()));
                        if(loc_reset_delta_sec < cs_timeout_sec_){
                            // TODO: set localization state
                        }
                        break;
                    }

                    default:
                        std::cout << "Unexpected control_strategy value = " << control_strategy << " with autonomy_level " << autonomy_level << "\n";
                        break;
                }
            }
        
            // TODO: call state machine update
        
            std::lock_guard<std::mutex> output_lock(dls_output_.mutex);
            // TODO: fill in dls_output_.dls_status_msg
        }

    }

    void PegasusOrchestrator::telemetryMain(const std::vector<dls2_interface::msg::EventLog> &events_to_publish)
    {
         {
            std::lock_guard<std::mutex> lock(dls_output_.mutex);
            dls_output_.dls_events_msg.events() = events_to_publish;    
            std::cout << "dls_output_.dls_events_msg.events: " << dls_output_.dls_events_msg.events().size() << "\n";
        }
        this->telemetry_manager_.tick(dls_input_, dls_output_);
    }

}
