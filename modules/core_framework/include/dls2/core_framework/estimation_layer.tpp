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
#ifndef ESTIMATION_LAYER_TPP_XGOZ3KJ8
#define ESTIMATION_LAYER_TPP_XGOZ3KJ8

// Include for benefit of IDEs, not necessary
#include "estimation_layer.hpp"

namespace dls
{
template <typename estimator_t>
bool EstimationLayer::addEstimator(const std::shared_ptr<estimator_t> &pEstimator)
{
	if(!std::is_base_of<Estimator, estimator_t>::value)
	{
		std::cout << "Error, estimator_t must inherit from Estimator" << std::endl;
		return false;
	}

	std::lock_guard<std::mutex> lock(this->estimators_mutex);
	this->estimators.insert
	(
		std::pair<Estimator::ID_t, std::shared_ptr<Estimator>>
		(
			pEstimator->getID(),
			pEstimator
		)
	);
	
	return true;
}
} // end namespace dls

#endif /* end of include guard: ESTIMATION_LAYER_TPP_XGOZ3KJ8 */
