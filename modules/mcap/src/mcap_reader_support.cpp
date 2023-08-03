#include "mcap_support/mcap_reader_support.hpp"

namespace dls
{
    namespace mcap_reader_support
    {
        /**
		 * @brief MCAPBaseTopic implementation
		 */
        MCAPBaseTopic::MCAPBaseTopic()
            : robot_(robotlib::RobotFactory::openRobot("aliengo"))
            , dds_participant_(std::make_shared<dls::DDSParticipant>("MCAPReaderSupport::signals", dls::domains::signals)) {}
        MCAPBaseTopic::~MCAPBaseTopic(){}

        /**
		 * @brief MCAPImu implementation
		 */
        MCAPImu::MCAPImu() : MCAPBaseTopic()
        {
            topic_name_ = "imu:trunk_imu";
            topic_type_ = dls::topicType(dls::topics::low_level_estimation::imu.first + ":trunk_imu", new ImuMsgPubSubType());
            signal_writer_ = std::make_shared<SignalWriter<Imu>>(dds_participant_, topic_type_, std::make_shared<Imu>());
        }
        MCAPImu::~MCAPImu(){}

        const std::string& MCAPImu::getTopicName() { return topic_name_; }
        void MCAPImu::fillMessage(const nlohmann::json& parsed_message)
        {
            for (auto item : parsed_message.items())
            {
                if((item.key()).compare("angular_velocity") == 0)
                {
                    int i{0};
                    for (const auto& value : item.value().items())
                    {
                        (*signal_writer_)->angular_velocity_(i) = value.value();
                        i++;
                    }
                }
                else if((item.key()).compare("angular_velocity_covariance") == 0)
                {
                    int i{0};
                    for (const auto& value : item.value().items())
                    {
                        (*signal_writer_)->angular_velocity_covariance_(i) = value.value();
                        i++;
                    }
                }
                else if((item.key()).compare("linear_acceleration") == 0)
                {
                    int i{0};
                    for (const auto& value : item.value().items())
                    {
                        (*signal_writer_)->linear_acceleration_(i) = value.value();
                        i++;
                    }
                }
                else if((item.key()).compare("linear_acceleration_covariance") == 0)
                {
                    int i{0};
                    for (const auto& value : item.value().items())
                    {
                        (*signal_writer_)->linear_acceleration_covariance_(i) = value.value();
                        i++;
                    }
                }
                else if((item.key()).compare("orientation") == 0)
                {
                    std::vector<double> values{};

                    for (const auto& value : item.value().items())
                    {
                        values.push_back(value.value());
                    }

                    (*signal_writer_)->orientation_.x() = values.at(0);
                    (*signal_writer_)->orientation_.y() = values.at(1);
                    (*signal_writer_)->orientation_.z() = values.at(2);
                    (*signal_writer_)->orientation_.w() = values.at(3);
                }
                else if((item.key()).compare("orientation_covariance") == 0)
                {
                    int i{0};
                    for (const auto& value : item.value().items())
                    {
                        (*signal_writer_)->orientation_covariance_(i) = value.value();
                        i++;
                    }
                }
                else if((item.key()).compare("timestamp") == 0)
                {
                    (*signal_writer_)->timestamp_ = item.value();
                }
                else if((item.key()).compare("frame_id") == 0)
                {
                    (*signal_writer_)->frame_id_ = item.value();
                }
            }
        }
        const std::shared_ptr<SignalWriter<Imu>> MCAPImu::getSignalWriter() { return signal_writer_; }

        /**
		 * @brief MCAPGaitSignal implementation
		 */
        MCAPGaitSignal::MCAPGaitSignal() : MCAPBaseTopic()
        {
            topic_name_ = "gait_signal";
            topic_type_ = dls::topicType(dls::topics::gait_signal.first, new GaitSignalMsgPubSubType());
            signal_writer_ = std::make_shared<SignalWriter<GaitSignal>>(dds_participant_, topic_type_, std::make_shared<GaitSignal>(robot_));
        }
        MCAPGaitSignal::~MCAPGaitSignal(){}

        const std::string& MCAPGaitSignal::getTopicName() { return topic_name_; }
        void MCAPGaitSignal::fillMessage(const nlohmann::json& parsed_message)
        {
            for (auto item : parsed_message.items())
            {
                if((item.key()).compare("joints_position") == 0)
                {
                    std::vector<double> values{};
                    for (const auto& value : item.value().items())
                    {            
                        values.push_back(value.value());                      
                    }

                    int leg_id{0};
                    for(auto &leg : (*signal_writer_)->desired_joints_position_)
                    {
                        int i{leg_id*leg.key_->getNJoints()};
                        for(auto &joint : *leg.data_)
                        {
                            (*signal_writer_)->desired_joints_position_[joint.key_] = values.at(i);
                            i++;
                        }
                        leg_id++;
                    }                              
                }

                if((item.key()).compare("joints_velocity") == 0)
                {
                    std::vector<double> values{};
                    for (const auto& value : item.value().items())
                    {            
                        values.push_back(value.value());                      
                    }

                    int leg_id{0};
                    for(auto &leg : (*signal_writer_)->desired_joints_velocity_)
                    {
                        int i{leg_id*leg.key_->getNJoints()};
                        for(auto &joint : *leg.data_)
                        {
                            (*signal_writer_)->desired_joints_velocity_[joint.key_] = values.at(i);
                            i++;
                        }
                        leg_id++;
                    }                              
                }
            }
        }
        const std::shared_ptr<SignalWriter<GaitSignal>> MCAPGaitSignal::getSignalWriter() { return signal_writer_; }

        /**
		 * @brief MCAPBlindState implementation
		 */
        MCAPBlindState::MCAPBlindState() : MCAPBaseTopic()
        {
            topic_name_ = "blind_state";
            topic_type_ = dls::topicType(dls::topics::low_level_estimation::blind_state.first, new BlindStateMsgPubSubType());
            signal_writer_ = std::make_shared<SignalWriter<BlindState>>(dds_participant_, topic_type_, std::make_shared<BlindState>(robot_));
        }
        MCAPBlindState::~MCAPBlindState(){}

        const std::string& MCAPBlindState::getTopicName() { return topic_name_; }
        void MCAPBlindState::fillMessage(const nlohmann::json& parsed_message)
        {
            for (auto item : parsed_message.items())
            {
                if((item.key()).compare("robot_name") == 0)
                {
                    (*signal_writer_)->robot_name_ = item.value();
                }

                if((item.key()).compare("joints_position") == 0)
                {
                    std::vector<double> values{};
                    for (const auto& value : item.value().items())
                    {            
                        values.push_back(value.value());                      
                    }

                    int leg_id = 0;
                    for(auto &leg : (*signal_writer_)->joints_position_)
                    {
                        int i = leg_id*leg.key_->getNJoints();
                        for(auto &joint : *leg.data_)
                        {
                            (*signal_writer_)->joints_position_[joint.key_] = values.at(i);
                            i++;
                        }
                        leg_id++;
                    }                              
                }

                if((item.key()).compare("joints_velocity") == 0)
                {
                    std::vector<double> values{};
                    for (const auto& value : item.value().items())
                    {            
                        values.push_back(value.value());                      
                    }

                    int leg_id = 0;
                    for(auto &leg : (*signal_writer_)->joints_position_)
                    {
                        int i = leg_id*leg.key_->getNJoints();
                        for(auto &joint : *leg.data_)
                        {
                            (*signal_writer_)->joints_velocity_[joint.key_] = values.at(i);
                            i++;
                        }
                        leg_id++;
                    }                              
                }

                if((item.key()).compare("stance_legs") == 0)
                {
                    std::vector<double> values{};
                    for (const auto& value : item.value().items())
                    {            
                        values.push_back(value.value());                      
                    }

                    int leg_id = 0;
                    for(auto &leg : (*signal_writer_)->joints_position_)
                    {
                        (*signal_writer_)->stance_legs_[leg.key_] = values.at(leg_id);  
                        leg_id++;                        
                    }                              
                }
            
                if((item.key()).compare("timestamp") == 0)
                {
                    (*signal_writer_)->timestamp_ = item.value();
                }
            }
        }
        const std::shared_ptr<SignalWriter<BlindState>> MCAPBlindState::getSignalWriter() { return signal_writer_; }

        /**
		 * @brief MCAPImu implementation
		 */
        MCAPT265Odometry::MCAPT265Odometry() : MCAPBaseTopic()
        {
            topic_name_ = "t265_odometry";
            topic_type_ = dls::topicType(dls::topics::high_level_estimation::t265_odometry.first, new T265OdometryMsgPubSubType());
            signal_writer_ = std::make_shared<SignalWriter<T265Odometry>>(dds_participant_, topic_type_, std::make_shared<T265Odometry>());
        }
        MCAPT265Odometry::~MCAPT265Odometry(){}

        const std::string& MCAPT265Odometry::getTopicName() { return topic_name_; }
        void MCAPT265Odometry::fillMessage(const nlohmann::json& parsed_message)
        {
            for (auto item : parsed_message.items())
            {
                if((item.key()).compare("position") == 0)
                {
                    int i{0};
                    for (const auto& value : item.value().items())
                    {
                        (*signal_writer_)->position_(i) = value.value();
                        i++;
                    }
                }

                if((item.key()).compare("linear_velocity") == 0)
                {
                    int i{0};
                    for (const auto& value : item.value().items())
                    {
                        (*signal_writer_)->linear_velocity_(i) = value.value();
                        i++;
                    }
                }

                if((item.key()).compare("angular_velocity") == 0)
                {
                    int i{0};
                    for (const auto& value : item.value().items())
                    {
                        (*signal_writer_)->angular_velocity_(i) = value.value();
                        i++;
                    }
                }
            
                if((item.key()).compare("orientation") == 0)
                {
                    std::vector<double> values{};

                    for (const auto& value : item.value().items())
                    {
                        values.push_back(value.value());
                    }

                    (*signal_writer_)->orientation_.x() = values.at(0);
                    (*signal_writer_)->orientation_.y() = values.at(1);
                    (*signal_writer_)->orientation_.z() = values.at(2);
                    (*signal_writer_)->orientation_.w() = values.at(3);
                }

                if((item.key()).compare("timestamp") == 0)
                {
                    (*signal_writer_)->timestamp_ = item.value();
                }
            }
        }
        const std::shared_ptr<SignalWriter<T265Odometry>> MCAPT265Odometry::getSignalWriter() { return signal_writer_; }

        /**
		 * @brief MCAPReaderSupport implementation
		 */
        MCAPReaderSupport::MCAPReaderSupport(){}
        MCAPReaderSupport::~MCAPReaderSupport(){}

        void MCAPReaderSupport::publishMessageOnTopic(mcap::LinearMessageView::Iterator& mcap_iterator, const nlohmann::json& parsed_message)
        {
            if((mcap_iterator->channel->topic).compare("imu:trunk_imu") == 0)
            {
                if(!mcap_topics_.mcap_topic_imu)
                {
                    mcap_imu_ = std::make_shared<mcap_reader_support::MCAPImu>();
                    mcap_topics_.mcap_topic_imu = true;
                }
                mcap_imu_->fillMessage(parsed_message);
                synchronizePublisher(mcap_iterator);
                (mcap_imu_->getSignalWriter())->publish();
            }
            else if((mcap_iterator->channel->topic).compare("gait_signal") == 0)
            {
                if(!mcap_topics_.mcap_topic_gait_signal)
                {
                    mcap_gait_signal_ = std::make_shared<mcap_reader_support::MCAPGaitSignal>();
                    mcap_topics_.mcap_topic_gait_signal = true;
                }
                mcap_gait_signal_->fillMessage(parsed_message);
                synchronizePublisher(mcap_iterator);
                (mcap_gait_signal_->getSignalWriter())->publish();
            }
            else if((mcap_iterator->channel->topic).compare("blind_state") == 0)
            {
                if(!mcap_topics_.mcap_topic_blind_state)
                {
                    mcap_blind_state_ = std::make_shared<mcap_reader_support::MCAPBlindState>();
                    mcap_topics_.mcap_topic_blind_state = true;
                }
                mcap_blind_state_->fillMessage(parsed_message);
                synchronizePublisher(mcap_iterator);
                (mcap_blind_state_->getSignalWriter())->publish();
            }
            else if((mcap_iterator->channel->topic).compare("t265_odometry") == 0)
            {
                if(!mcap_topics_.mcap_topic_t265_odometry)
                {
                    mcap_t265_odometry_ = std::make_shared<mcap_reader_support::MCAPT265Odometry>();
                    mcap_topics_.mcap_topic_t265_odometry = true;
                }
                mcap_t265_odometry_->fillMessage(parsed_message);
                synchronizePublisher(mcap_iterator);
                (mcap_t265_odometry_->getSignalWriter())->publish();
            }
            else
            {
                synchronizePublisher(mcap_iterator);
            }
            time_ = std::chrono::steady_clock::now();
            previous_timestamp_ = mcap_iterator->message.logTime;
        }

        void MCAPReaderSupport::synchronizePublisher(mcap::LinearMessageView::Iterator& mcap_iterator)
        {
            if(mcap_iterator->message.sequence > 1)
            {
                auto elapsed_time = std::chrono::duration_cast<std::chrono::nanoseconds>(std::chrono::steady_clock::now() - time_).count();

                // You consider two elements for slowing down the print of a new message "M_i":
                // (1) "T_M_i - T_M_i-1". That is the difference of timestamps (nanoseconds) from message "i" and message "i-1"
                // (2) the elapsed time (nanoseconds) from the beginning of the for loop to the moment in which you print the message
                // You get "W_P = (1) - (2)" (nanoseconds) and publish immediately or sleep for "W_P" (if greater than zero) nanoseconds before printing the message  
                wait_for_publishing_ = (mcap_iterator->message.logTime - previous_timestamp_) - elapsed_time;

                if (wait_for_publishing_ > 0)
                {
                    std::this_thread::sleep_for(std::chrono::nanoseconds(wait_for_publishing_));
                }
            }
        }
    } // namespace mcap_reader_support
} //namespace dls