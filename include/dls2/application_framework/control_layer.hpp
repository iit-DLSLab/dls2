/*******************************************************************************
*                                                       ,----,                 *
*                                                     .'   .' \                *
*                                                   ,----,'    |               *
*               ________  ___       ________        |    :  .  ;               *
*              |\   ___ \|\  \     |\   ____\       ;    |.'  /                *
*              \ \  \_|\ \ \  \    \ \  \___|_      `----'/  ;                 *
*               \ \  \ \\ \ \  \    \ \_____  \       /  ;  /                  *
*                \ \  \_\\ \ \  \____\|____|\  \     ;  /  /-,                 *
*                 \ \_______\ \_______\____\_\  \   /  /  /.`|                 *
*                  \|_______|\|_______|\_________\./__;      :                 *
*                                     \|_________||   :    .'                  *
*                                                 ;   | .'                     *
*                                                 `---'                        *
********************************************************************************
* Author:            Hendrik de Bruin                                          *
* Maintainer:        Hendrik de Bruin                                          *
* author email:      hendrik.debruin@iit.it                                    *
*******************************************************************************/
#ifndef CONTROL_LAYER_HPP_YCHFNYBM
#define CONTROL_LAYER_HPP_YCHFNYBM

// =============================================================================
// Includes
// =============================================================================
#include "dls2/application_framework/app_layer.hpp"
#include "dls2/controller/controller.hpp"
#include "dls2/gait_generator/gait_generator.hpp"

#include <map>
#include <memory>
#include <mutex>
#include <thread>
#include <atomic>

#include "dls2/util/messaging/subscriber_base.hpp"
#include "dls2/util/messaging/publisher_base.hpp"
#include "dls2/msg/control_signalPubSubTypes.h"
#include "dls2/msg/desired_torquesPubSubTypes.h"
#include "dls2/msg/hello_worldPubSubTypes.h"
#include "dls2/math/spline/ramp.hpp"
#include "dls2/command/command.hpp"

// temporary console includes
// TODO( "REMOVE THESE INCLUDES WHEN A BETTER CONSOLE IS MADE")
#include "dls2/msg/stringmsgPubSubTypes.h"
#include "dls2/topics/activate_controller.hpp"
#include "dls2/topics/deactivate_controller.hpp"
#include "dls2/topics/activate_gait_generator.hpp"
#include "dls2/topics/deactivate_gait_generator.hpp"
#include "dls2/util/log/log.hpp"

// =============================================================================
// Class Interface
// =============================================================================
namespace dls
{
class ControlLayer : public AppLayer
{
public:
	ControlLayer();
	~ControlLayer();

	// ========================== Interface Overrides ==========================
	Status run() override;
	// TODO("This function is not implemented")
	Status shutdown() override;

	// ============================== Controllers ==============================
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
	/// Activates a gait generator
	///
	/// This will stop any other running gait generators
	/// @ret true if the controller exists, false otherwise. See also
	/// ControlLayer::deactivateGaitGenerators
	bool activateGaitGenerator(const GaitGenerator::ID_t&);

	/// Deactivates the current gait generator
	void deactivateGaitGenerators();

private:

	// TODO("This should be put in the robot class")
	/// Saturates torques so that they do not exceed safe limits
	///
	/// @param req The requested torques
	/// @ret A saturated version of the torques that do not exceed safe limits
	Eigen::MatrixXd saturateTorques(const Eigen::MatrixXd &req) const;

	/// Helper class that subscribes to a given controller's control signal
	///
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
	// TODO Change type of time
	/// Publish Torques to the rest of the framework
	///
	void publishDesiredTorques(const Eigen::VectorXd &, double time) const;

	// ============================= Data Members ==============================
	// BEGIN critical section
		/// Helper struct for collecting controllers, their thread handles, and
		/// subscribers to a controller's control signal
		///
		struct ControllerData
		{
			ControllerData
			(
				std::shared_ptr<spline::SplineBase<double>> pSpline_in,
				std::shared_ptr<spline::SplineBase<double>> pSpline_out,
				const std::chrono::duration<double> &duration_in,
				const std::chrono::duration<double> &duration_out
			);

			/*const*/ pid_t controller_pid;
			/*const*/ std::shared_ptr<ControlSubListener> pSubscriber;
			/*const*/ Controller::ID_t ID;
			std::atomic<double> premultiplier; ///< Spline value to premutilply the torque signal
			const std::chrono::duration<double> spline_in_duration;
			const std::chrono::duration<double> spline_out_duration;
			const std::shared_ptr<spline::SplineBase<double>> pSpline_in;
			const std::shared_ptr<spline::SplineBase<double>> pSpline_out;
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

	std::atomic_bool should_quit;

	// BEGIN critical section
		std::vector<std::thread> wait_on_controller_threads;
		std::mutex wait_on_controller_threads_mutex;
	// END critical section

	/// Default controller spline-in
	///
	std::shared_ptr<spline::Ramp<double>> pDefault_spline_in;

	/// Default controller spline-out
	///
	std::shared_ptr<spline::Ramp<double>> pDefault_spline_out;

	/// Default spline duration
	///
	std::chrono::seconds default_duration_seconds;

	void waitOnChildController(std::shared_ptr<ControllerData>);
	void waitOnChildGaitGenerator(std::shared_ptr<GaitGeneratorData>);
	void deactivateController(std::shared_ptr<ControllerData> pData);

	CommandManager command_manager;

	// ================================ Members ================================
	logging::coutstream scout;
	logging::clogstream clog;
	logging::cfatalstream cfatal;
};
} // end namespace dls

#include "dls2/application_framework/control_layer.tpp"

#endif /* end of include guard: CONTROL_LAYER_HPP_YCHFNYBM */
