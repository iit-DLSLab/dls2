
#ifndef CONTROL_LAYER_HPP_YCHFNYBM
#define CONTROL_LAYER_HPP_YCHFNYBM

// =============================================================================
// Includes
// =============================================================================
#include "dls2/application/layer.hpp"
#include "dls2/controller/controller.hpp"
#include "dls2/motion_generator/motion_generator.hpp"
#include "dls2/msg_wrappers/signal_writer.hpp"
#include "dls2/msg_wrappers/desired_torques.hpp"

#include <map>
#include <memory>
#include <mutex>
#include <thread>
#include <atomic>
#include <boost/process.hpp>

#include "dls2/util/messaging/dds_reader.hpp"
#include "dls2/util/messaging/dds_writer.hpp"
#include "dls2/math/ramp.hpp"
#include "dls2/log/log.hpp"

#include <pthread.h>

// =============================================================================
// Class Interface
// =============================================================================
namespace dls
{

/// Control layer
///
/// Responsible for managing controllers and motion generators
class ControlLayer : public Layer
{
	typedef void * (*THREADFUNCPTR)(void *);
	
public:
	/// Constructor	
	ControlLayer(std::string ID, std::string robot_name);

	/// Destructor
	~ControlLayer();

	/// @brief  run method overide
	/// @return status of the control layer.
	AppStatus run() override;

	/// @brief stop method override
	/// @return current status of the control layer.
	AppStatus stop() override;

	/// Loads a controller
	/// @return true if the controller loads correctly.
	bool loadController(const std::string&);

	/// Unloads a controller
	/// @return true if the controller unloads correctly.
	bool unloadController(const std::string&);

	/// Loads a motion generator
	/// @return true if the generator loaded correctly.
	bool loadMotionGenerator(const std::string&);

	/// Unloads the current motion generator
	/// @return true if the generator unloads correctly.
	bool unloadMotionGenerator(const std::string&);

	/// Returns the last published desired torques
	robotlib::JointState getPublishedDesiredTorques();

	/// Outputs info about the control layer
	/// @return returns a string with all the info.
	std::string where() override;

private:
	// TODO("This should be put in the robot class")
	/// Saturates torques so that they do not exceed safe limits
	///
	/// @param req The requested torques
	/// @ret A saturated version of the torques that do not exceed safe limits
	robotlib::JointState saturateTorques(const robotlib::JointState& req) const;

	// ============================= Data Members ==============================
	// BEGIN critical section
		std::map<std::string, std::shared_ptr<ControllerData>> controllers;
		std::mutex controllers_mutex;
	// END critical section

	// BEGIN critical section
		std::map<std::string, std::shared_ptr<AppData>> motion_generators;
		std::mutex motion_mutex;
	// END critical section
	
	std::shared_ptr<dls::DDSParticipant> ddsSignalLink;
	
	/// Default controller spline-in
	///
	std::shared_ptr<math::Ramp<double>> pDefault_spline_in;

	/// Default controller spline-out
	///
	std::shared_ptr<math::Ramp<double>> pDefault_spline_out;

	/// Default spline duration
	///
	std::chrono::seconds default_duration_seconds;

	std::shared_ptr<robotlib::RobotBase> pRobot;

	/// @brief Output signals
	SignalWriter<DesiredTorques> control_signal;

	DesiredTorques torques;

	bool unloadController(std::shared_ptr<ControllerData> pData);

	bool unloadMotionGenerator(std::shared_ptr<AppData> pData);

	// Real-time thread that gather all the control sinals and sends to robot
	pthread_t controlSignalGatherThread;

	static void *controlSignalGather(void *data);

	// ================================ Members ================================
	logging::clogstream clog;
	logging::cfatalstream cfatal;
};

} // end namespace dls

#endif /* end of include guard: CONTROL_LAYER_HPP_YCHFNYBM */
