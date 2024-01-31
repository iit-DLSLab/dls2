
#ifndef MOTION_GENERATOR_HPP_5MDX0BG2
#define MOTION_GENERATOR_HPP_5MDX0BG2

// =============================================================================
// Includes
// =============================================================================
#include "dls2/application/periodic_app.hpp"
#include "dls2/util/messaging/dds_participant.hpp"

#include "robotlib/robot_factory.hpp"

#include "yaml-cpp/yaml.h"

// =============================================================================
// Class Interface
// =============================================================================
namespace dls
{
class MotionGenerator : public PeriodicApp
{
	public:
		typedef MotionGenerator *create_t(std::string);
		typedef void destroy_t(MotionGenerator*);

		// Constructors
		MotionGenerator
		(
			const std::string&,								///< The ID of the motion generator
			const std::shared_ptr<robotlib::RobotBase>&	///< A pointer to the robot model
		);

		virtual ~MotionGenerator();

		std::shared_ptr<dls::DDSParticipant> getParticipant();
		std::shared_ptr<const robotlib::RobotBase> getRobot(); 

		virtual void goHome();
		virtual void goFold();
		virtual void runGoHome() = 0;
		virtual void runGoFold() = 0;
		virtual void stopMotion() = 0;
		/*!
		@brief Function to inizialize motion generation data before activation and/or in the constructor
		*/
		virtual void init() = 0;
		virtual void runPostures();


	protected:
		/// Function gets called each epoch.
        ///
        /// @param time The time when this function is called,
        virtual void run(const std::chrono::system_clock::time_point &time) = 0;

		/// Pointer to the robot model
		const std::shared_ptr<robotlib::RobotBase> pRobot;

		robotlib::JointState home_configuration;
        robotlib::JointState fold_configuration;

		void setHomeConfiguration();

		void setFoldConfiguration();

		void setConsoleFunctions();

		//! Logic variable to enable goHome procedure
		bool go_home_;

		//! Logic variable to enable goFold procedure
		bool go_fold_;

	private:
		std::shared_ptr<dls::DDSParticipant> ddsLink;
	};
} // end namespace dls

#endif /* end of include guard: MOTION_GENERATOR_HPP_5MDX0BG2 */