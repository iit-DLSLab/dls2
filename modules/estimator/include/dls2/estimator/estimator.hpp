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

#include "dls2/application/periodic_app.hpp"
#include "robotlib/robot_base.hpp"

namespace dls
{
	class Estimator : public PeriodicApp
	{
	public:
		using ID_t = std::string;
		Estimator
		(
			const ID_t&		///< The ID of this estimator
		);
		
		virtual ~Estimator() = default;

		typedef Estimator *create_t(std::string);
		typedef void destroy_t(Estimator*);

		std::shared_ptr<dls::DDSParticipant> getParticipant();

	private:
		std::shared_ptr<dls::DDSParticipant> signalLink;		
	};
} // end namespace dls

#endif /* end of include guard: ESTIMATOR_HPP_I8Z0QRSO */