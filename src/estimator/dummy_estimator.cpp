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
********************************************************************************
* Author:            Hendrik de Bruin                                          *
* Maintainer:        Hendrik de Bruin                                          *
* author email:      hendrik.debruin@iit.it                                    *
*******************************************************************************/
#include "dls2/estimator/dummy_estimator.hpp"
#include "dls2/util/debug/debug.hpp"

using namespace dls;
DummyEstimator::DummyEstimator() :
	Estimator("Dummy Estimator", std::chrono::duration<double>(1))
{ }

void DummyEstimator::run(const std::chrono::system_clock::time_point&)
{ }

extern "C" Estimator *create()
{
	auto p = new DummyEstimator;
	return p;
}

extern "C" void destroy(Estimator *p)
{
	delete p;
}
