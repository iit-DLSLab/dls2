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
*******************************************************************************/
#ifndef CONTROL_LAYER_HPP_YCHFNYBM
#define CONTROL_LAYER_HPP_YCHFNYBM

// =============================================================================
// Includes
// =============================================================================
#include "app_layer.hpp"
#include "dls2/controller/controller.hpp"
#include "dls2/motion_generator/motion_generator.hpp"

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

#define SCHED_DEADLINE       6
#define __NR_sched_setattr           314
#define __NR_sched_getattr           315


struct sched_attr {
     __u32 size;

     __u32 sched_policy;
     __u64 sched_flags;

     /* SCHED_NORMAL, SCHED_BATCH */
     __s32 sched_nice;

     /* SCHED_FIFO, SCHED_RR */
     __u32 sched_priority;

     /* SCHED_DEADLINE (nsec) */
     __u64 sched_runtime;
     __u64 sched_deadline;
     __u64 sched_period;
};

// =============================================================================
// Class Interface
// =============================================================================
namespace dls
{

/// Control layer
///
/// Responsible for managing controllers and motion generators
class ControlLayer : public AppLayer
{
	typedef void * (*THREADFUNCPTR)(void *);
	
public:
	ControlLayer(std::string ID);
	~ControlLayer();

	// ========================== Interface Overrides ==========================
	Status run() override;

	Status shutdown() override;

	// ============================== Controllers ==============================
	/// Loads a controller
	///
	/// @ret true if the controller exists, false otherwise.
	/// See also ControlLayer::unloadController
	bool loadController(const std::string&);

	/// Unloads a controller
	///
	/// @ret true if the controller exists, false otherwise
	/// See also ControlLayer::loadController
	bool unloadController(const std::string&);

	// ============================ Motion Generators ============================
	/// Loads a motion generator
	///
	/// This will stop any other running motion generators
	/// @ret true if the controller exists, false otherwise. See also
	/// ControlLayer::unloadMotionGenerators
	bool loadMotionGenerator(const std::string&);

	/// Unloads the current motion generator
	///
	bool unloadMotionGenerator(const std::string&);

	/// Returns the last published desired torques
	///
	Eigen::VectorXd getPublishedDesiredTorques();

	std::string where() override;
private:

	// TODO("This should be put in the robot class")
	/// Saturates torques so that they do not exceed safe limits
	///
	/// @param req The requested torques
	/// @ret A saturated version of the torques that do not exceed safe limits
	Eigen::MatrixXd saturateTorques(const Eigen::MatrixXd &req) const;

	// ============================ Communincation =============================
	// TODO Change type of time
	/// Publish Torques to the rest of the framework
	///
	void publishDesiredTorques(const Eigen::VectorXd &, double time);

	// ============================= Data Members ==============================
	// BEGIN critical section
		std::map<std::string, std::shared_ptr<ControllerData>> controllers;
		std::mutex controllers_mutex;
	// END critical section

	// BEGIN critical section
		std::map<std::string, std::shared_ptr<AppData>> motion_generators;
		std::mutex motion_mutex;
	// END critical section
	
	dls::DDSParticipant* ddsLink;
	
	/// Default controller spline-in
	///
	std::shared_ptr<math::Ramp<double>> pDefault_spline_in;

	/// Default controller spline-out
	///
	std::shared_ptr<math::Ramp<double>> pDefault_spline_out;

	/// Default spline duration
	///
	std::chrono::seconds default_duration_seconds;

	// BEGIN critical section
		std::mutex last_published_desired_torques_mutex;
		Eigen::VectorXd last_published_desired_torques;
	// END critical section

	bool unloadController(std::shared_ptr<ControllerData> pData);

	bool unloadMotionGenerator(std::shared_ptr<AppData> pData);

	// Real-time thread that gather all the control sinals and sends to robot
	pthread_t controlSignalGatherThread;

	static void *controlSignalGather(void *data);

	std::shared_ptr<robotlib::RobotBase> pRobot;

	// ================================ Members ================================
	logging::clogstream scout;
	logging::clogstream clog;
	logging::cfatalstream cfatal;
};

} // end namespace dls

#endif /* end of include guard: CONTROL_LAYER_HPP_YCHFNYBM */
