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
#ifndef SPLINE_BASE_HPP_ERGQ2YJO
#define SPLINE_BASE_HPP_ERGQ2YJO

namespace dls
{

template <typename T, typename index_t = double>
class SplineBase
{
public:
	SplineBase(index_t max_index = 0, index_t min_index = 0);
	virtual T eval(index_t t) = 0;

	const index_t max_index;
	const index_t min_index;
};

} // namespace dls

#endif /* end of include guard: SPLINE_BASE_HPP_ERGQ2YJO */
