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
	// template <typename controller_t>
	// void addController(const std::shared_ptr<controller_t>&);

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
	// template <typename generator_t>
	// void addGaitGenerator(const std::shared_ptr<generator_t>&);

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
			ControllerData() :
				controller_pid(0),
				pSubscriber(nullptr),
				ID()
			{ }
			pid_t controller_pid;
			std::shared_ptr<ControlSubListener> pSubscriber;
			Controller::ID_t ID;
		};
		std::map<Controller::ID_t, std::shared_ptr<ControllerData>> controllers_b;
		std::mutex controllers_mutex_b;
	// END critical section
	// BEGIN critical section
		struct GaitGeneratorData
		{
			pid_t gait_generator_pid;
			// std::shared_ptr<std::thread> pGait_generator_wait_thread;
		};
		std::shared_ptr<GaitGeneratorData> pGait_generator_data;
		std::mutex gait_generators_mutex;
	// END critical section
	PublisherBase<DesiredTorquesMsgPubSubType> publisher;

	// BEGING critical section
		// size_t num_children;
		// std::condition_variable num_children_cv;
		// std::mutex num_children_mutex;
	// END critical section
	std::atomic_bool should_quit;

	// BEGIN critical section
		std::vector<std::thread> wait_on_controller_threads;
		std::mutex wait_on_controller_threads_mutex;
	// END critical section

	void waitOnChildController(std::shared_ptr<ControllerData>);
	void waitOnChildGaitGenerator(std::shared_ptr<GaitGeneratorData>);
	void deactivateController(std::shared_ptr<ControllerData> pData);
};

#include "application_framework/control_layer.tpp"

#endif /* end of include guard: CONTROL_LAYER_HPP_YCHFNYBM */
