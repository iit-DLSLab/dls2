
#include "dls2/estimator/estimator.hpp"

using namespace dls;
// =============================================================================
// Constructors
// =============================================================================
Estimator::Estimator(const ID_t &ID) 
	: PeriodicApp(ID)
	, signalLink(std::make_shared<dls::DDSParticipant>(
		"Estimator::" + ID,
		dls::domains::signals))
{ }

std::shared_ptr<dls::DDSParticipant> Estimator::getParticipant()
{
	return this->signalLink;
}
