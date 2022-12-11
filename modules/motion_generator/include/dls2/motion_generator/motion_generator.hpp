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

// =============================================================================
// Class Interface
// =============================================================================
namespace dls
{
class MotionGenerator : public PeriodicAppLayerComponent
{
	protected:
		// TODO this is repeated in Controller. Refactor this into one location
		typedef std::chrono::duration<double, std::ratio<1, 1'000'000>> period_t;

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

		virtual ~MotionGenerator() = default;

		dls::DDSParticipant* getParticipant();
		const robotlib::RobotBase* getRobot(); 

	protected:
		/// Function gets called each epoch.
        ///
        /// @param time The time when this function is called,
        virtual void run(const std::chrono::system_clock::time_point &time) = 0;

	private:
		/// Pointer to the robot model
		const std::shared_ptr<const robotlib::RobotBase> pRobot;	

		dls::DDSParticipant ddsLink;		
	};
} // end namespace dls

#endif /* end of include guard: MOTION_GENERATOR_HPP_5MDX0BG2 */