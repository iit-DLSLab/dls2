#ifndef PLUGIN_HPP
#define PLUGIN_HPP

#include "dls2/application/periodic_app.hpp"
#include "dls_messages/wrapper_base.hpp"
#include "dls2/signal/signal_writer.hpp"
#include "dls2/signal/signal_reader.hpp"

#include <mutex>
namespace dls
{
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
      * @tparam MsgWrapperType class name of the wrapper handling the message associated to the input topic
      * @tparam constructor_args_types types of the constructor arguments of the MsgWrapperType class
      * @param[in] topic topic to subscribe to
      * @param[in] input pointer to the variable storing the last read input
      * @param[in] auxiliary_callback auxiliary function to be called when a new message is received
      */
    template <typename MsgWrapperType>
    void buildInput(const dls::topicType &topic, WrapperBase *input,
                    const std::function<void()> &auxiliary_callback = std::function<void()>([](){}),
                    bool required_on_activation = true);

    /*!
      * @brief Add an input to the plugin.
      * @details
      * When calling this function, it is created a new data reader subscribed to the input topic and it is stored the pointer to the input WrapperBase variable.
      * @tparam MsgWrapperType class name of the wrapper handling the message associated to the input topic
      * @tparam constructor_args_types types of the constructor arguments of the MsgWrapperType class
      * @param[in] name input name
      * @param[in] topic topic to subscribe to
      * @param[in] input pointer to the variable storing the last read input
      * @param[in] auxiliary_callback auxiliary function to be called when a new message is received
      */
    template <typename MsgWrapperType>
    void buildInput(const std::string &name, const dls::topicType &topic, WrapperBase *input,
                    const std::function<void()> &auxiliary_callback = std::function<void()>([](){}),
                    bool required_on_activation = true);

    /*!
      * @brief Add an output to the plugin.
      * @details
      * When calling this function, it is created a new writer publishing on the input topic and it is stored the pointer to the output WrapperBase variable.
      * @tparam MsgWrapperType class name of the wrapper handling the message associated to the input topic
      * @tparam constructor_args_types types of the constructor arguments of the MsgWrapperType class
      * @param[in] topic topic to subscribe to
      * @param[in] output pointer to the variable storing the last wrote output
      */
    template <typename MsgWrapperType>
    void buildOutput(const dls::topicType &topic, WrapperBase *output);

    /*!
      * @brief Add an output to the plugin.
      * @details
      * When calling this function, it is created a new writer publishing on the input topic and it is stored the pointer to the output WrapperBase variable.
      * @tparam MsgWrapperType class name of the wrapper handling the message associated to the input topic
      * @tparam constructor_args_types types of the constructor arguments of the MsgWrapperType class
      * @param[in] name name of the output
      * @param[in] topic topic to subscribe to
      * @param[in] output pointer to the variable storing the last wrote output
      */
    template <typename MsgWrapperType>
    void buildOutput(const std::string &name, const dls::topicType &topic, WrapperBase *output);

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

    std::shared_ptr<SignalReaderBase> getReader(const std::string &name);

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
      * @brief Check for new requests/replies in the registered services.
      * @details
      * When calling this function, the active services are checked for new requests/replies.
      */
    void updateServices();

    void* getData(const std::string &serviceName);

    /*!
      * @brief Remove a service from the plugin.
      * @details
      * When calling this function, the service associated with the name will be removed.
      * @param[in] name name of the service
      */
    void deleteServices();

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

    //! Vector of inputs (data readers)
    std::vector<std::shared_ptr<SignalReaderBase>> readers_;
    //! Vector of outputs (data writers)
    std::vector<std::shared_ptr<SignalWriterBase>> writers_;

    //! Map of data readers with their corresponding outputs variable. It is populated when calling buildInput function with the input name as additional argument
    std::map<std::string, std::pair<std::shared_ptr<SignalReaderBase>, WrapperBase*>> readers_map_;
    //! Map of data writers with their corresponding outputs variable. It is populated when calling buildOutput function with the output name as additional argument
    std::map<std::string, std::pair<std::shared_ptr<SignalWriterBase>, WrapperBase*>> writers_map_;
    private:
    //! Vector of pointers pointing to input variables: created when adding an input with buildInput function
    std::vector<WrapperBase *> inputs_;
    //! Vector of pointers pointing to output variables: created when adding an output with buildOutput function
    std::vector<WrapperBase *> outputs_;
    // ! Check if inputs are required on activation
    std::vector<bool> are_inputs_required_on_activation;

    std::map<std::string, std::shared_ptr<RpcService>> _rpc_srvc_map;
  };
} // end namespace dls

#include "dls2/plugin/plugin.tpp"

#endif /* PLUGIN_HPP */
