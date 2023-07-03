#ifndef MCAP_READER_SUPPORT_HPP
#define MCAP_READER_SUPPORT_HPP

#include "mcap/reader.hpp"

#include "robotlib/robot_factory.hpp"

#include "dls2/util/messaging/dds_participant.hpp"
#include "dls2/msg_wrappers/signal_writer.hpp"

#include "dls2/msg_wrappers/imu.hpp"
#include "dls2/msg_wrappers/gait_signal.hpp"
#include "dls2/msg_wrappers/blind_state.hpp"
#include "dls2/msg_wrappers/odometry.hpp"

#include <nlohmann/json.hpp>

namespace dls
{
    namespace mcap_reader_support
    {
        /**
		 * @brief MCAPBaseTopic header
		 */
        class MCAPBaseTopic
        {
        public:
            MCAPBaseTopic();
            virtual ~MCAPBaseTopic();

            virtual const std::string& getTopicName() = 0;
            virtual void fillMessage(const nlohmann::json& parsed_message) = 0;

        protected:
            std::shared_ptr<robotlib::RobotBase> robot_;
		    std::shared_ptr<dls::DDSParticipant> dds_participant_;
            std::string topic_name_{};
            dls::topicType topic_type_{};
        };

        /**
		 * @brief MCAPImu header
		 */
        class MCAPImu : public MCAPBaseTopic
        {
        public:
            MCAPImu();
            virtual ~MCAPImu();

            virtual const std::string& getTopicName() override;
            virtual void fillMessage(const nlohmann::json& parsed_message) override;
            const std::shared_ptr<SignalWriter<Imu>> getSignalWriter();

        private:
            std::shared_ptr<SignalWriter<Imu>> signal_writer_;
        };

        /**
		 * @brief MCAPGaitSignal header
		 */
        class MCAPGaitSignal : public MCAPBaseTopic
        {
        public:
            MCAPGaitSignal();
            virtual ~MCAPGaitSignal();

            virtual const std::string& getTopicName() override;
            virtual void fillMessage(const nlohmann::json& parsed_message) override;
            const std::shared_ptr<SignalWriter<GaitSignal>> getSignalWriter();

        private:
            std::shared_ptr<SignalWriter<GaitSignal>> signal_writer_;
        };

        /**
		 * @brief MCAPBlindState header
		 */
        class MCAPBlindState : public MCAPBaseTopic
        {
        public:
            MCAPBlindState();
            virtual ~MCAPBlindState();

            virtual const std::string& getTopicName() override;
            virtual void fillMessage(const nlohmann::json& parsed_message) override;
            const std::shared_ptr<SignalWriter<BlindState>> getSignalWriter();

        private:
            std::shared_ptr<SignalWriter<BlindState>> signal_writer_;
        };

        /**
		 * @brief MCAPT265Odometry header
		 */
        class MCAPT265Odometry : public MCAPBaseTopic
        {
        public:
            MCAPT265Odometry();
            virtual ~MCAPT265Odometry();

            virtual const std::string& getTopicName() override;
            virtual void fillMessage(const nlohmann::json& parsed_message) override;
            const std::shared_ptr<SignalWriter<Odometry>> getSignalWriter();

        private:
            std::shared_ptr<SignalWriter<Odometry>> signal_writer_;
        };

        /**
		 * @brief MCAPReaderSupport header
		 */
        class MCAPReaderSupport
        {
        public:
            MCAPReaderSupport();
            ~MCAPReaderSupport();

            void synchronizePublisher(mcap::LinearMessageView::Iterator& mcap_iterator);
            void publishMessageOnTopic(mcap::LinearMessageView::Iterator& mcap_iterator, const nlohmann::json& parsed_message);

        private:
            std::shared_ptr<mcap_reader_support::MCAPImu> mcap_imu_;
            std::shared_ptr<mcap_reader_support::MCAPGaitSignal> mcap_gait_signal_;
            std::shared_ptr<mcap_reader_support::MCAPBlindState> mcap_blind_state_;
            std::shared_ptr<mcap_reader_support::MCAPT265Odometry> mcap_t265_odometry_;

            struct
            {
                bool mcap_topic_imu{false};
                bool mcap_topic_gait_signal{false};
                bool mcap_topic_blind_state{false};
                bool mcap_topic_t265_odometry{false};
            } mcap_topics_;

            /**
             * @brief Used to compute the time passed after different MCAP messages, so as to set the sleep for synchronizing the published messages 
             */
            std::chrono::steady_clock::time_point time_{};
            /**
    		 * @brief Used to store the information regarding the previous MCAP message timestamp, needed for synchronizing the published messages
    		 */
    		uint64_t previous_timestamp_{};
            /**
             * @brief Used to compute the amount of time that has to be waited before printing/publishing a new MCAP message
             */
            uint64_t wait_for_publishing_{};
        };
    } //namespace mcap_reader_support
} //namespace dls

#endif