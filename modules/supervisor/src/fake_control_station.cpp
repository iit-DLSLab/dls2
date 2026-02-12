

#include "dls2/supervisor/fake_control_station.hpp"

#include <iostream>
#include <thread>
#include <chrono>
#include <queue>
#include <mutex>

void FakeControlStation::inputThread() {
    int value;
    while (std::cin >> value) {
        std::lock_guard<std::mutex> lock(mtx);
        commands.push(value);
    }
}

namespace dls
{
    FakeControlStation::FakeControlStation(const std::string &ID)
        : dls::PeriodicAppPlugin(ID)
    {
        // Telemetry-related readers and writers definition
        cs_basic_command_writer = std::make_shared<dls::Writer<dls2_interface::msg::CSBasicCommand>>(this->dds_participant_, dls::topics::cs_basic_command);
        cs_emergency_writer = std::make_shared<dls::Writer<dls2_interface::msg::CSEmergency>>(this->dds_participant_, dls::topics::cs_emergency);
        target_position_writer = std::make_shared<dls::Writer<dls2_interface::msg::TargetPosition>>(this->dds_participant_, dls::topics::cs_target_position);
        stay_out_zones_writer = std::make_shared<dls::Writer<dls2_interface::msg::StayOutZones>>(this->dds_participant_, dls::topics::cs_stay_out_zones);
        reference_path_writer = std::make_shared<dls::Writer<dls2_interface::msg::ReferencePath>>(this->dds_participant_, dls::topics::cs_reference_path);
        feet_reference_writer = std::make_shared<dls::Writer<dls2_interface::msg::FeetReference>>(this->dds_participant_, dls::topics::cs_feet_reference);
        base_reference_writer = std::make_shared<dls::Writer<dls2_interface::msg::BaseReference>>(this->dds_participant_, dls::topics::cs_base_reference);
        joint_states_writer = std::make_shared<dls::Writer<dls2_interface::msg::JointStates>>(this->dds_participant_, dls::topics::cs_joint_states);
        cs_loc_reset_writer = std::make_shared<dls::Writer<dls2_interface::msg::BaseReference>>(this->dds_participant_, dls::topics::cs_loc_reset);
        command_call_writer = std::make_shared<dls::Writer<dls2_interface::msg::CommandCall>>(this->dds_participant_, dls::topics::cs_command_call);
    
        std::cout << "FakeControlStation built\n";

        std::thread input(&FakeControlStation::inputThread, this);
        input.detach();
    }

    void FakeControlStation::send_emergency_on_command(){
            cs_emergency_writer->msg.header().timestamp() = toNs<unsigned long long>(std::chrono::system_clock::now());
            cs_emergency_writer->msg.header().sequence_id() = (cs_basic_command_writer->msg.header().sequence_id() + 1) % MAX_SEQUENCE_ID;
            cs_emergency_writer->msg.emergency_stop() = true;
            cs_emergency_writer->publish();
            std::cout << "cs: sending cs_emergency msg: true\n";
    }

    void FakeControlStation::send_emergency_off_command(){
            cs_emergency_writer->msg.header().timestamp() = toNs<unsigned long long>(std::chrono::system_clock::now());
            cs_emergency_writer->msg.header().sequence_id() = (cs_basic_command_writer->msg.header().sequence_id() + 1) % MAX_SEQUENCE_ID;
            cs_emergency_writer->msg.emergency_stop() = false;
            cs_emergency_writer->publish();
            std::cout << "cs: sending cs_emergency msg: false\n";
    }

     void FakeControlStation::send_target_position_command(){
        target_position_writer->msg.header().timestamp() = toNs<unsigned long long>(std::chrono::system_clock::now());
        target_position_writer->msg.header().sequence_id() = (cs_basic_command_writer->msg.header().sequence_id() + 1) % MAX_SEQUENCE_ID;
        target_position_writer->msg.target_position().x() = 10.0;
        target_position_writer->msg.target_position().y() = 10.0;
        target_position_writer->publish();
        std::cout << "cs: sending target_position msg\n";
    }

    void FakeControlStation::send_stay_out_zones_command(){
        stay_out_zones_writer->msg.header().timestamp() = toNs<unsigned long long>(std::chrono::system_clock::now());
        stay_out_zones_writer->msg.header().sequence_id() = (stay_out_zones_writer->msg.header().sequence_id() + 1) % MAX_SEQUENCE_ID;
        stay_out_zones_writer->msg.zones().resize(1);
        stay_out_zones_writer->msg.zones()[0].markers().resize(3);
        for(dls2_interface::msg::Position2D& p : stay_out_zones_writer->msg.zones()[0].markers()){
            p.x() = 1.0;
            p.y() = 1.0;
        }
        stay_out_zones_writer->publish();
        std::cout << "cs: sending stay_out_zones msg\n";
    }

    void FakeControlStation::send_reference_path_command(){
        reference_path_writer->msg.header().timestamp() = toNs<unsigned long long>(std::chrono::system_clock::now());
        reference_path_writer->msg.header().sequence_id() = (reference_path_writer->msg.header().sequence_id() + 1) % MAX_SEQUENCE_ID;
        reference_path_writer->msg.path().resize(5);
        auto init_coord_x = 1.0;
        auto init_coord_y = 1.0;
        for(dls2_interface::msg::Position2D& p : reference_path_writer->msg.path()){
            p.x() = init_coord_x++;
            p.y() = init_coord_y++;
        }
        reference_path_writer->publish();
        std::cout << "cs: sending reference_path msg\n";
    }

    void FakeControlStation::send_feet_reference_command(){
        feet_reference_writer->msg.header().timestamp() = toNs<unsigned long long>(std::chrono::system_clock::now());
        feet_reference_writer->msg.header().sequence_id() = (feet_reference_writer->msg.header().sequence_id() + 1) % MAX_SEQUENCE_ID;
        feet_reference_writer->msg.feet_ids().resize(1);
        feet_reference_writer->msg.feet_ids()[0] = 3;
        feet_reference_writer->msg.feet_position().resize(1);
        feet_reference_writer->msg.feet_position()[0].x() = 10.0;
        feet_reference_writer->msg.feet_position()[0].y() = 13.0;
        feet_reference_writer->msg.feet_position()[0].z() = 0.3;
        feet_reference_writer->publish();
        std::cout << "cs: sending feet_reference msg\n";
    }

    void FakeControlStation::send_base_reference_command(){
        base_reference_writer->msg.header().timestamp() = toNs<unsigned long long>(std::chrono::system_clock::now());
        base_reference_writer->msg.header().sequence_id() = (base_reference_writer->msg.header().sequence_id() + 1) % MAX_SEQUENCE_ID;
        std::array<double, 3> linear = {1.0, 2.0, 3.0};
        std::array<double, 3> angular = {1.0, 2.0, 3.0};
        base_reference_writer->msg.target_state().velocity().linear() = linear;
        base_reference_writer->msg.target_state().velocity().angular() = angular;
        base_reference_writer->publish();
        std::cout << "cs: sending base_reference msg\n";
    }

    void FakeControlStation::send_joint_states_command(){
        joint_states_writer->msg.header().timestamp() = toNs<unsigned long long>(std::chrono::system_clock::now());
        joint_states_writer->msg.header().sequence_id() = (joint_states_writer->msg.header().sequence_id() + 1) % MAX_SEQUENCE_ID;
        joint_states_writer->msg.joints().effort().resize(2);
        for(double& effort : joint_states_writer->msg.joints().effort()){
            effort = 10.0;
        }
        joint_states_writer->publish();
        std::cout << "cs: sending joint_state msg\n";
    }

    void FakeControlStation::send_cs_loc_reset_command(){
        cs_loc_reset_writer->msg.header().timestamp() = toNs<unsigned long long>(std::chrono::system_clock::now());
        cs_loc_reset_writer->msg.header().sequence_id() = (cs_loc_reset_writer->msg.header().sequence_id() + 1) % MAX_SEQUENCE_ID;
        std::array<double, 3> position = {1.0, 2.0, 3.0};
        std::array<double, 4> orientation = {1.0, 2.0, 3.0, 4.0};
        cs_loc_reset_writer->msg.target_state().pose().position() = position;
        cs_loc_reset_writer->msg.target_state().pose().orientation() = orientation;
        cs_loc_reset_writer->publish();
        std::cout << "cs: sending loc_reset msg\n";

    }

    void FakeControlStation::send_command_call_command(){
        command_call_writer->msg.header().timestamp() = toNs<unsigned long long>(std::chrono::system_clock::now());
        command_call_writer->msg.header().sequence_id() = (command_call_writer->msg.header().sequence_id() + 1) % MAX_SEQUENCE_ID;
        command_call_writer->publish();
        std::cout << "cs: sending command_call msg\n";
    }

    void FakeControlStation::autonomy_level_manual(){
        autonomy_level_ = AutonomyLevel::MANUAL;
        std::cout << "cs: autonomy_level to MANUAL\n";
    }

    void FakeControlStation::autonomy_level_autonomous(){
        autonomy_level_ = AutonomyLevel::AUTONOMOUS;
        std::cout << "cs: autonomy_level to AUTONOMOUS\n";
    }

    void FakeControlStation::locomotion_strategy_mpc(){
        locomotion_strategy_ = LocomotionStrategy::MPC;
        std::cout << "cs: locomotion_strategy to MPC\n";
    }

    void FakeControlStation::locomotion_strategy_rl(){
        locomotion_strategy_ = LocomotionStrategy::RL;
        std::cout << "cs: locomotion_strategy to RL\n";
    }

    void FakeControlStation::control_strategy_target_position(){
        control_strategy_ = ControlStrategy::TARGET_POSITION;
        std::cout << "cs: control_strategy to TARGET_POSITION\n";
    }

    void FakeControlStation::control_strategy_reference_path(){
        control_strategy_ = ControlStrategy::REFERENCE_PATH;
        std::cout << "cs: control_strategy to REFERENCE_PATH\n";
    }

    void FakeControlStation::control_strategy_stay_out_zones(){
        control_strategy_ = ControlStrategy::STAY_OUT_ZONES;
        std::cout << "cs: control_strategy to STAY_OUT_ZONES\n";
    }

    void FakeControlStation::control_strategy_feet_reference(){
        control_strategy_ = ControlStrategy::FEET_REFERENCE;
        std::cout << "cs: control_strategy to FEET_REFERENCE\n";
    }

    void FakeControlStation::control_strategy_base_reference(){
        control_strategy_ = ControlStrategy::BASE_REFERENCE;
        std::cout << "cs: control_strategy to BASE_REFERENCE\n";
    }

    void FakeControlStation::control_strategy_joint_reference(){
        control_strategy_ = ControlStrategy::JOINT_REFERENCE;
        std::cout << "cs: control_strategy to JOINT_REFERENCE\n";
    }

    void FakeControlStation::control_strategy_loc_reset(){
        control_strategy_ = ControlStrategy::LOC_RESET;
        std::cout << "cs: control_strategy to LOC_RESET\n";
    }


    void FakeControlStation::run(const std::chrono::system_clock::time_point &time)
    {
        // cs_basic_command
        cs_basic_command_writer->msg.header().timestamp() = toNs<unsigned long long>(std::chrono::system_clock::now());
        cs_basic_command_writer->msg.header().sequence_id() = (cs_basic_command_writer->msg.header().sequence_id() + 1) % MAX_SEQUENCE_ID;
        cs_basic_command_writer->msg.autonomy_level() = to_underlying(autonomy_level_);
        cs_basic_command_writer->msg.locomotion_strategy() = to_underlying(locomotion_strategy_);
        cs_basic_command_writer->msg.control_strategy() = to_underlying(control_strategy_);
        cs_basic_command_writer->msg.enable() = true;
        cs_basic_command_writer->publish();

        // Execute all pending commands (non-blocking)
        while (true) {
            int cmd = 0;
            {
                std::lock_guard<std::mutex> lock(mtx);
                if (commands.empty()){
                    break;
                }
                cmd = commands.front();
                commands.pop();
            }

            switch (cmd) {
                case 1: send_emergency_on_command(); break;
                case 2: send_emergency_off_command(); break;
                case 3: send_target_position_command(); break;
                case 4: send_stay_out_zones_command(); break;
                case 5: send_reference_path_command(); break;
                case 6: send_feet_reference_command(); break;
                case 7: send_base_reference_command(); break;
                case 8: send_joint_states_command(); break;
                case 9: send_cs_loc_reset_command(); break;
                case 10: send_command_call_command(); break;

                case 11: autonomy_level_manual(); break;
                case 12: autonomy_level_autonomous(); break;
                case 13: locomotion_strategy_mpc(); break;
                case 14: locomotion_strategy_rl(); break;
                case 15: control_strategy_target_position(); break;
                case 16: control_strategy_reference_path(); break;
                case 17: control_strategy_stay_out_zones(); break;
                case 18: control_strategy_feet_reference(); break;
                case 19: control_strategy_base_reference(); break;
                case 20: control_strategy_joint_reference(); break;
                case 21: control_strategy_loc_reset(); break;

                default: break;
            }
        }
    }
}
