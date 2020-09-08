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
#ifndef RAMP_HPP_WOXJEKFS
#define RAMP_HPP_WOXJEKFS

#include "dls2/math/spline/spline_base.hpp"
namespace dls
{
namespace spline
{

/// Simple ramp function
///
// The ramp is clamped like this:
//          ______
//         /
//        /
// ______/
template <typename domain_t = double>
class Ramp : public SplineBase<domain_t>
{
public:
	Ramp
	(
		const domain_t &min_domain = 0,
		const domain_t &max_domain = 1,
		const domain_t &min_range = 0,
		const domain_t &max_range = 1
	);

	/// Evaluate the ramp
	///
	/// @param t the point in the domain to evaluate this ramp.
	/// If `t` is outside of the domain, the ramp will be clamped at its minimum
	/// or maximum value
	virtual domain_t eval(domain_t t) override;

private:
	domain_t min_range;
	domain_t max_range;
};

} // namespace spline
} // namespace dls

#include "dls2/math/spline/ramp.tpp"

#endif /* end of include guard: IDENTITY_SPLINE_HPP_ZETKQVB4 */
