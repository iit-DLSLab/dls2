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
#ifndef SCREW_HPP_O9FQ1FIE
#define SCREW_HPP_O9FQ1FIE

#include <Eigen/Dense>

#include "msg/screw.h"

/// Wrapper class for screw messages
///
/// A screw is encapsulated as a 6x1 vector. The first 3 components are linear,
/// the final 3 components are angular
class Screw
{
public:
	/// Default constructor
	///
	/// Assigns memory, but does not zero it
	Screw();

	/// Converting Constructor
	///
	Screw(const ScrewMsg&);

	operator ScrewMsg() const;

	/// Sets the linear component of the screw
	///
	void setLinear(const Eigen::Vector3d&);

	/// Sets the angular component of the screw
	///
	void setAngular(const Eigen::Vector3d&);

	/// Returns a reference to the internal representation of the screw
	///
	/// Note that this is a returned reference. Do not use this reference after
	/// the Screw instance which owns it has gone out of scope
	Eigen::Matrix<double, 6, 1> &data();

private:

	/// Data storage
	///
	Eigen::Matrix<double, 6, 1> eigen_vec;
};

#endif /* end of include guard: SCREW_HPP_O9FQ1FIE */
