#ifndef PLUGIN_HPP
#define PLUGIN_HPP

#include "dls2/application/periodic_app.hpp"
#include "dls2/signal/writer.hpp"
#include "dls2/signal/reader.hpp"

#include <mutex>
namespace dls
{
	struct InputInfo
	{
		double latest_period_ms{};
		std::chrono::steady_clock::time_point latest_timestamp{};
		uint32_t latest_sequence_id { 0 }; 
		bool got_first_sequence_id{ false };
		bool sequence_id_sane{ true };
		bool are_inputs_required_on_activation{ false };
		std::string topic_name{""};
		std::shared_ptr<ReaderBase> reader{};
	};

	struct OutputInfo
	{
		public:
			std::shared_ptr<WriterBase> writer;
			std::string topic_name{""};

			uint32_t nextSequenceId()
			{
				return ++sequence_id % MAX_SEQUENCE_ID;
			}

		private:
			uint32_t sequence_id{ 0 };
	};

	/*!
	 * @class Plugin
	 * @brief This is a base class for creating plugins for either apps or periodic apps.
	 */
	class Plugin
	{
	public:
		/*!
		 * @brief Constructor.
		 * @param[in] ID name of plugin
		 * @param[in] domain domain the plugin belongs to
		 */
		Plugin(const std::string &ID, const domainType &domain);

    /*!
      * @brief Destructor.
      */
    virtual ~Plugin();

		/*!
		 * @brief Add an input to the plugin.
		 * @details
		 * When calling this function, it is created a new data reader subscribed to the input topic and it is stored the pointer to the input WrapperBase variable.
		 * @tparam MsgType class name of the wrapper handling the message associated to the input topic
		 * @tparam constructor_args_types types of the constructor arguments of the MsgType class
		 * @param[in] topic topic to subscribe to
		 * @param[in] input pointer to the variable storing the last read input
		 * @param[in] auxiliary_callback auxiliary function to be called when a new message is received
		 */
		template <typename MsgType>
		dls::ReaderPtr<MsgType> buildInput(const dls::topicType &topic, const std::function<void()> &auxiliary_callback = std::function<void()>([](){}), bool required_on_activation = true);

		/*!
		 * @brief Add an output to the plugin.
		 * @details
		 * When calling this function, it is created a new writer publishing on the input topic and it is stored the pointer to the output WrapperBase variable.
		 * @tparam MsgType class name of the wrapper handling the message associated to the input topic
		 * @tparam constructor_args_types types of the constructor arguments of the MsgType class
		 * @param[in] topic topic to subscribe to
		 * @param[in] output pointer to the variable storing the last wrote output
		 */
		template <typename MsgType>
		dls::WriterPtr<MsgType> buildOutput(const dls::topicType &topic);

    /*!
      * @brief Read all the inputs.
      * @details
      * When calling this function, each reader updates the corresponding input variable with the last read message
      */
    void read();

    /*!
      * @brief Read a specific the inputs.
      * @details
      * When calling this function, the reader associated to the input name updates the corresponding input variable with the last read message
      * @param[in] name name of the input to read
      */
    void read(const std::string &name);

    /*!
      * @brief Write all the outputs.
      * @details
      * When calling this function, each writer writes the corresponding output to the associated output topic. This function updates also the timestamp of the output, if it has one.
      */
    void write();

    /*!
      * @brief Write a specific output.
      * @details
      * When calling this function, the writer associated to the output_name writes the corresponding output to the associated output topic. This function updates also the timestamp of the output, if it has one.
      * @param[in] name name of the output to write
      */
    void write(const std::string &name);

	protected:
		//! Domain participant of the plugin
		std::shared_ptr<dls::DDSParticipant> dds_participant_;

    std::mutex unique_outputs_mutex;
    std::condition_variable unique_outputs_cv;

    /*! @brief Check if the inputs are receiving data*/
    bool areInputsReceivingData();

    /*! @brief Check if there is no other data writers publishing on the same topics of the outputs*/
    bool areOutputsUnique();

    std::stringstream missing_inputs;
    std::stringstream common_outputs;

    /*!
     * @brief Add service and requester associated with a service name.
     * @details
     * When calling this function, a new service is created and the service type is registered.
     * Furthermore, a replier is created to reply on received requests associated with this service.
     * @note In DLS2, services only have one replier.
     * @param[in] name name of the service
     * @param[in] requestTopic topic for the request
     * @param[in] replyTopic topic for the reply
     */
    void createReplier(const std::string &serviceName,
                       const dls::topicType &replyTopic,
                       const dls::topicType &requestTopic,
                       void* data);

    void createRequester(const std::string &serviceName,
                         const dls::topicType &replyTopic,
                         const dls::topicType &requestTopic,
                         void* data);

    void sendRequest(const std::string& serviceName, void* data);

    void sendReply(const std::string &serviceName, void* data);

    /*!
      * @brief Check for new request/reply data for the registered services.
      * @details
      * When calling this function, the active services are checked for new data.
      */
    void* getData(const std::string &serviceName);

    /*!
      * @brief Remove all services from the plugin.
      * @details
      * When calling this function, all services will be removed.
      */
    void deleteServices();

		std::mutex input_info_mutex_;
		std::mutex output_info_mutex_;

		//! Vector of inputs (data readers)
		std::vector<InputInfo> input_info_{};
		//! Vector of outputs (data writers)
		std::vector<OutputInfo> output_info_{};

		// Map from topic (input) name to id in the inputs vector
		std::map<std::string, size_t> inputs_map;
		// Map from topic (output) name to id in the outputs vector
		std::map<std::string, size_t> outputs_map;

	private:
		
		/*! @brief Updating input sanity check-related data */
		void updateInputInfo(InputInfo& input_info);

		/*! @brief Sanity check on msg sequence id, wrapping included */
		bool checkSequenceId(uint32_t prev_sequence_id, uint32_t received_sequence_id, int delta_sample_count);

    std::map<std::string, std::shared_ptr<RpcService>> _rpc_srvc_map;

	};
} // end namespace dls

#include "dls2/plugin/plugin.tpp"

#endif /* PLUGIN_HPP */
