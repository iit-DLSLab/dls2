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
#include "estimator/dummy_estimator.hpp"
#include "util/debug/debug.hpp"

dls::DummyEstimator::DummyEstimator() :
	Estimator("Dummy Estimator", std::chrono::duration<double>(1))
{ }

void dls::DummyEstimator::run(const std::chrono::system_clock::time_point&)
{ }

extern "C" dls::Estimator *create()
{
	auto p = new dls::DummyEstimator;
	return p;
}

extern "C" void destroy(dls::Estimator *p)
{
	delete p;
}
