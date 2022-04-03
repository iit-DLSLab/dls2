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

namespace dls
{
class Estimator : public PeriodicAppLayerComponent
{
public:
	using ID_t = std::string;
	Estimator
	(
		const ID_t&,		///< The ID of this estimator
		const period_t&		///< The period of this estimator
	);
	virtual ~Estimator() = default;

	typedef Estimator *create_t();
	typedef void destroy_t(Estimator*);

	// ID_t getID() const;

private:
	// const ID_t ID;
};
} // end namespace dls

#endif /* end of include guard: ESTIMATOR_HPP_I8Z0QRSO */
