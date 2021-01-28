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
#ifndef SPLINE_BASE_HPP_ERGQ2YJO
#define SPLINE_BASE_HPP_ERGQ2YJO

namespace dls
{
namespace spline
{

/// Spline base class
///
/// A spline maps \f$ [\texttt{MIN_DOMAIN}, \texttt{MAX_DOMAIN}] \mapsto [0, 1]
/// \f$. The domains \f$ (-\infty, \texttt{MIN_DOMAIN}) \f$ and \f$
/// (\texttt{MAX_DOMAIN}, \infty) \f$ are undefined
template <typename domain_t = double>
class SplineBase
{
public:
	// SplineBase(const domain_t &min_domain = 0, const domain_t &max_domain = 1);
	// TODO move the constructor to the tpp file
	SplineBase(const domain_t &min_domain = 0, const domain_t &max_domain = 1) :
		max_domain(max_domain),
		min_domain(min_domain)
	{ }

	virtual ~SplineBase() = default;
	virtual domain_t eval(domain_t t) = 0;

protected:
	const domain_t max_domain;
	const domain_t min_domain;
};
} // namespace spline
} // namespace dls

#include "dls2/math/spline/spline_base.tpp"

#endif /* end of include guard: SPLINE_BASE_HPP_ERGQ2YJO */
