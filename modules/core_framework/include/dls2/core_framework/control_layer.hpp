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
#include "dls2/gait_generator/gait_generator.hpp"

#include <map>
#include <memory>
#include <mutex>
#include <thread>
#include <atomic>
#include <boost/process.hpp>

#include "dls2/util/messaging/dds_reader.hpp"
#include "dls2/util/messaging/dds_writer.hpp"
#include "dls2/math/spline/ramp.hpp"
#include "dls2/log/log.hpp"

// =============================================================================
// Class Interface
// =============================================================================
namespace dls
{
/// Control layer
///
/// Responsible for managing controllers and gait generators
class ControlLayer : public AppLayer
{
public:
	ControlLayer(std::string ID, bool *should_quit);
	~ControlLayer();

	// ========================== Interface Overrides ==========================
	Status run() override;

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

	// ============================ Gait Generators ============================
	/// Activates a gait generator
	///
	/// This will stop any other running gait generators
	/// @ret true if the controller exists, false otherwise. See also
	/// ControlLayer::deactivateGaitGenerators
	bool activateGaitGenerator(const GaitGenerator::ID_t&);

	/// Deactivates the current gait generator
	void deactivateGaitGenerator();

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
				const std::chrono::duration<double> &duration_out,
				uint controlSize
			);

			ControlSignal getLastPublishedControlSignal();
	
			boost::process::child *proc;
			std::shared_ptr<DDSReader> dds_reader;
			Controller::ID_t ID;
			std::atomic<double> premultiplier; ///< Spline value to premutilply the torque signal
			const std::chrono::duration<double> spline_in_duration;
			const std::chrono::duration<double> spline_out_duration;
			const std::shared_ptr<spline::SplineBase<double>> pSpline_in;
			const std::shared_ptr<spline::SplineBase<double>> pSpline_out;

			// BEGIN critical section	
				ControlSignal control_signal;
				std::mutex control_signal_mutex;
			// END critical section
		};
		std::map<Controller::ID_t, std::shared_ptr<ControllerData>> controllers_b;
		std::mutex controllers_mutex_b;
	// END critical section


	// BEGIN critical section
		struct GaitGeneratorData
		{
			GaitGeneratorData() 
				: ID()
				, proc(nullptr)
			{ }
			GaitGenerator::ID_t ID;
			boost::process::child *proc;
		};

		GaitGeneratorData gaitData;
		std::mutex gaitMutex;
	// END critical section
	
	dls::DDSWriter *ddsControl;
	dls::DDSWriter *ddsMonitor;	

	/// Default controller spline-in
	///
	std::shared_ptr<spline::Ramp<double>> pDefault_spline_in;

	/// Default controller spline-out
	///
	std::shared_ptr<spline::Ramp<double>> pDefault_spline_out;

	/// Default spline duration
	///
	std::chrono::seconds default_duration_seconds;

	void deactivateController(std::shared_ptr<ControllerData> pData);

	// ================================ Members ================================
	logging::coutstream scout;
	logging::clogstream clog;
	logging::cfatalstream cfatal;
};
} // end namespace dls

#endif /* end of include guard: CONTROL_LAYER_HPP_YCHFNYBM */
