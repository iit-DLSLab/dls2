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
#include "dls2/estimator/dummy_estimator.hpp"
#include "dls2/util/debug/debug.hpp"

using namespace dls;
DummyEstimator::DummyEstimator() :
	Estimator("Dummy Estimator", std::chrono::duration<double>(1))
{ }

void DummyEstimator::run(const std::chrono::system_clock::time_point&)
{ }

std::string DummyEstimator::where()
{
	return "Dude, don't ask me that";
}

extern "C" Estimator *create()
{
	auto p = new DummyEstimator;
	return p;
}

extern "C" void destroy(Estimator *p)
{
	delete p;
}
