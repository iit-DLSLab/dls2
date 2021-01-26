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
#ifndef ESTIMATION_LAYER_HPP_3QHYDR67
#define ESTIMATION_LAYER_HPP_3QHYDR67

#include <memory>
#include <map>
#include <mutex>
#include <atomic>
#include <string>
#include <thread>

#include "app_layer.hpp"
#include "dls2/estimator/estimator.hpp"

namespace dls
{
// TODO build and document this class
/// Estimation layer
///
/// Manages estimators
class EstimationLayer : public AppLayer
{
public:
	EstimationLayer();
	~EstimationLayer();

	Status run() override;
	Status shutdown() override;

	template <typename estimator_t>
	void addEstimator(const std::shared_ptr<estimator_t>&);

	void loadEstimator(const std::string&);

	//TODO("These two should probably return bool")
	bool activateEstimator(const Estimator::ID_t&);
	bool deactivateEstimator(const Estimator::ID_t);

	std::string where() override {return "not yet implemented"; }

private:
	// BEGIN critical section
		std::map<Estimator::ID_t, std::shared_ptr<Estimator>> estimators;
		std::map<Estimator::ID_t, std::thread> estimator_threads;
		std::mutex estimators_mutex;
	// END critical section
	std::atomic_bool should_run;
};
} // end namespace dls

#include "estimation_layer.tpp"

#endif /* end of include guard: ESTIMATION_LAYER_HPP_3QHYDR67 */
