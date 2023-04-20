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
#ifndef ESTIMATION_LAYER_HPP
#define ESTIMATION_LAYER_HPP

#include <memory>
#include <map>
#include <mutex>
#include <atomic>
#include <string>
#include <thread>

#include "dls2/core/app_layer.hpp"
#include "dls2/estimator/estimator.hpp"
#include "dls2/components/app_data.hpp"

#include <pthread.h>

namespace dls
{
// TODO build and document this class
/// Estimation layer

	class EstimationLayer : public AppLayer
	{
	public:
		EstimationLayer(std::string ID);
		~EstimationLayer();

		Status run() override;
		Status stop() override;

		bool loadEstimator(const Estimator::ID_t&);
		bool unloadEstimator(const Estimator::ID_t&);

		int numOfEstimators();

		std::string where() override {return "not yet implemented"; }

	private:
		// BEGIN critical section
			std::map<Estimator::ID_t, std::shared_ptr<AppData>> estimators;
			std::mutex estimators_mutex;
		// END critical section

		std::shared_ptr<dls::DDSWriter> ddsMonitor;
	};
} // end namespace dls

#endif /* end of include guard: ESTIMATION_LAYER_HPP */
