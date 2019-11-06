#ifndef CONTROL_LAYER_HPP_YCHFNYBM
#define CONTROL_LAYER_HPP_YCHFNYBM

// =============================================================================
// Includes
// =============================================================================
#include "application_framework/app_layer.hpp"
#include "controller/controller.hpp"
#include "gait_generator/gait_generator.hpp"

#include <map>
#include <memory>
#include <mutex>
#include <thread>

#include "util/messaging/subscriber_base.hpp"
#include "util/messaging/publisher_base.hpp"
#include "msg/control_signalPubSubTypes.h"
#include "msg/desired_torquesPubSubTypes.h"
#include "msg/hello_worldPubSubTypes.h"

// =============================================================================
// Class Interface
// =============================================================================
class ControlLayer : public AppLayer
{
public:
	ControlLayer();
	~ControlLayer();

	// ========================== Interface Overrides ==========================
	Status run() override;
	TODO("This function is not implemented")
	Status shutdown() override;

	// ============================== Controllers ==============================
	/// Adds a controller to the control layer
	///
	/// This call does not start the controller. see
	/// ControlLayer::activateController
	template <typename controller_t>
	void addController(const std::shared_ptr<controller_t>&);

	/// Activates a controller
	///
	/// @ret true if the controller exists, false otherwise.
	/// See also ControlLayer::deactivateController
	bool activateController(const Controller::ID_t&);

	/// Deactivates a controller
	///
	/// @ret true if the controller exists, false otherwise
	/// See also ControlLayer::activateController
	bool deactivateController(const Controller::ID_t&);

	/// Dynamically loads a controller at run time
	///
	/// This function throws a std::runtime_error if the controller shared
	/// object cannot be found
	void loadController(const std::string &name);

	// ============================ Gait Generators ============================
	/// Adds a gait generator to the control layer
	///
	/// This call does not start the gait generator. See
	/// ControlLayer::activateGaitGenerator
	template <typename generator_t>
	void addGaitGenerator(const std::shared_ptr<generator_t>&);

	/// Activates a gait generator
	///
	/// This will stop any other running gait generators
	/// @ret true if the controller exists, false otherwise. See also
	/// ControlLayer::deactivateGaitGenerators
	bool activateGaitGenerator(const GaitGenerator::ID_t&);

	/// Deactivates the current gait generator
	void deactivateGaitGenerators();

	/// Dynamically loads a gait generator at run time
	///
	/// This function throws a std::runtime_error if the gait generator shared
	/// object cannot be found
	void loadGaitGenerator(const std::string &name);

private:
	TODO("This should be put in the robot class")
	/// Saturates torques so that they do not exceed safe limits
	///
	/// @param req The requested torques
	/// @ret A saturated version of the torques that do not exceed safe limits
	Eigen::MatrixXd saturateTorques(const Eigen::MatrixXd &req) const;

	/// Helper class that subscribes to a given controller's control signal
	class ControlSubListener : public SubscriberBase<ControlSignalMsgPubSubType>
	// class ControlSubListener : public SubscriberBase<HelloWorldPubSubType>
	{
	public:
		ControlSubListener(const std::string &topic);
		~ControlSubListener() = default;

		// Disallow move and copy
		// This is because pSubscriber gets the address `this`. If this object
		// is moved into ControlLayer's control_subscribers, pSubscriber would
		// then point to invalid memory
		ControlSubListener(const ControlSubListener&&) = delete;
		ControlSubListener(const ControlSubListener&) = delete;

		std::shared_ptr<ControlSignal> getLastPublishedControlSignal();
		void onNewDataMessage(eprosima::fastrtps::Subscriber *sub) override;
	private:
		// BEGIN critical section
			std::shared_ptr<ControlSignal> control_signal;
			std::mutex control_signal_mutex;
		// END critical section
		eprosima::fastrtps::SampleInfo_t info;
	};
	// ============================ Communincation =============================
	void publishDesiredTorques(const Eigen::VectorXd &) const;

	// ============================= Data Members ==============================
	// BEGIN critical section
		/// Helper struct for collecting controllers, their thread handles, and
		/// subscribers to a controller's control signal
		struct ControllerData
		{
			std::shared_ptr<Controller> pController;
			std::shared_ptr<std::thread> pExecution_thread;
			std::shared_ptr<ControlSubListener> pSubscriber;
		};
		std::map<Controller::ID_t, ControllerData> controllers_b;
		std::mutex controllers_mutex_b;
	// END critical section
	// BEGIN critical section
		std::map<GaitGenerator::ID_t, std::shared_ptr<GaitGenerator>> generators;
		std::mutex gait_generators_mutex;
		// ALWAYS check if this is nullptr
		TODO("rename with underscores")
		std::shared_ptr<GaitGenerator> currentActiveGenerator;
		std::thread active_generator_thread;
	// END critical section
	PublisherBase<DesiredTorquesMsgPubSubType> publisher;
};

#include "application_framework/control_layer.tpp"

#endif /* end of include guard: CONTROL_LAYER_HPP_YCHFNYBM */
