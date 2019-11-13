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
#ifndef ESTIMATOR_HPP_I8Z0QRSO
#define ESTIMATOR_HPP_I8Z0QRSO

#include "todo.h"
#include <string>

#include "application_framework/components/periodic_app_layer_component.hpp"

TODO("Document this class")
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

	ID_t getID() const;

private:
	const ID_t ID;
};


#endif /* end of include guard: ESTIMATOR_HPP_I8Z0QRSO */
