#ifndef GO_HOME_HPP
#define GO_HOME_HPP

#include "dls2/action/action_server_base.hpp"
#include "dls2/msg_wrappers/vector3d.hpp"
#include "dls2/msg_wrappers/trajectory_generator.hpp"
#include "dls2/msg_wrappers/bool.hpp"

namespace dls
{
	// =========================================================================
	// Go Home action server
	// =========================================================================
	/*! @brief A class implementing the Go Home action server.
	 */
	class GoHome : public ActionServerBase<Vector3d, TrajectoryGenerator, Bool>
	{
	public:
		/*! @brief Constructor
		 */
		GoHome(const std::shared_ptr<robotlib::RobotBase> pRobot);

		/*! @brief Destructor
		 */
		~GoHome() = default;

		virtual void runAction() override;
		int count = 0;
	}; // end namespace dls
}
#endif /* end of include guard: GO_HOME_HPP */