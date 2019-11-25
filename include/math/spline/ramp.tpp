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
#ifndef RAMP_TPP_RPS6XN7J
#define RAMP_TPP_RPS6XN7J

#include "math/spline/ramp.hpp"

namespace dls
{
namespace spline
{
template <typename domain_t>
Ramp<domain_t>::Ramp
(
	const domain_t &min_domain_,
	const domain_t &max_domain_,
	const domain_t &min_range_,
	const domain_t &max_range_
):
	SplineBase<domain_t>(min_domain_, max_domain_),
	min_range(min_range_),
	max_range(max_range_)
{ }

template <typename domain_t>
domain_t Ramp<domain_t>::eval(domain_t t)
{
	if(t <= this->min_domain) return this->min_domain;
	if(t >= this->max_domain) return this->max_domain;

	// return (t - this->min_domain)/(this->max_domain - this->min_domain);
	return
		(
			(this->max_range - this->min_range)
			/
			(this->max_domain - this->min_domain)
		)
		*
		(
			t - this->min_domain
		)
		+ this->min_range;
}
} // namepsace spline
} // namespace dls

#endif /* end of include guard: IDENTITY_SPLINE_TPP_OICD8WMA */
