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

#include <boost/process.hpp>
#include <pthread.h>

namespace dls
{
// TODO build and document this class
/// Estimation layer

	class EstimatorData
    {
    public:
        EstimatorData();
        ~EstimatorData();

        std::shared_ptr<boost::process::child> proc;
        std::shared_ptr<DDSReader> dds_reader;
        std::string ID;
    };

	class EstimationLayer : public AppLayer
	{
	public:
		EstimationLayer(std::string ID);
		~EstimationLayer();

		Status run() override;
		Status shutdown() override;

		bool loadEstimator(const Estimator::ID_t&);
		bool removeEstimator(const Estimator::ID_t&);

		int numOfEstimators();

		std::string where() override {return "not yet implemented"; }

	private:
		// BEGIN critical section
			std::map<Estimator::ID_t, std::shared_ptr<EstimatorData>> estimators;
			std::mutex estimators_mutex;
		// END critical section

		dls::DDSWriter *ddsMonitor;	
	};
} // end namespace dls

#endif /* end of include guard: ESTIMATION_LAYER_HPP_3QHYDR67 */
