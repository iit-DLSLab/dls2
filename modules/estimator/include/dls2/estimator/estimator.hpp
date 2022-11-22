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
#ifndef ESTIMATOR_HPP_I8Z0QRSO
#define ESTIMATOR_HPP_I8Z0QRSO

#include <string>

#include "dls2/components/periodic_app_layer_component.hpp"
#include "robotlib/robot_base.hpp"

namespace dls
{
	class Estimator : public PeriodicAppLayerComponent
	{
	public:
		using ID_t = std::string;
		Estimator
		(
			const ID_t&,		///< The ID of this estimator
			const std::shared_ptr<robotlib::RobotBase>&,
			const period_t&		///< The period of this estimator
		);
		
		virtual ~Estimator() = default;

		typedef Estimator *create_t(std::shared_ptr<robotlib::RobotBase>);
		typedef void destroy_t(Estimator*);

	protected:
		const std::shared_ptr<robotlib::RobotBase> pRobot;
		dls::DDSParticipant signalLink;		
	};
} // end namespace dls

#endif /* end of include guard: ESTIMATOR_HPP_I8Z0QRSO */