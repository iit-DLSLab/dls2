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
#ifndef MOTION_GENERATOR_HPP_5MDX0BG2
#define MOTION_GENERATOR_HPP_5MDX0BG2

// =============================================================================
// Includes
// =============================================================================
#include "dls2/components/periodic_app_layer_component.hpp"
#include "dls2/util/messaging/dds_participant.hpp"

#include "robotlib/robot_factory.hpp"

#include "yaml-cpp/yaml.h"

// =============================================================================
// Class Interface
// =============================================================================
namespace dls
{
class MotionGenerator : public PeriodicAppLayerComponent
{
	public:
		typedef MotionGenerator *create_t(std::string);
		typedef void destroy_t(MotionGenerator*);

		// Constructors
		MotionGenerator
		(
			const std::string&,								///< The ID of the motion generator
			const std::shared_ptr<robotlib::RobotBase>&,	///< A pointer to the robot model
			const period_t& 								///< The period of the motion generator
		);

		virtual ~MotionGenerator();

		std::shared_ptr<dls::DDSParticipant> getParticipant();
		std::shared_ptr<const robotlib::RobotBase> getRobot(); 

		virtual void goHome();
		virtual void goFold();
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

		void setHomeConfiguration(YAML::Node& config, const std::string& data_name);

		void setFoldConfiguration(YAML::Node& config, const std::string& data_name);

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