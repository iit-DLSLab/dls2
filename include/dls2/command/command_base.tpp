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
#ifndef COMMAND_BASE_TPP_VNPJCDBU
#define COMMAND_BASE_TPP_VNPJCDBU

// =============================================================================
// Includes
// =============================================================================
#include "dls2/command/command_base.hpp"

namespace dls
{
// =============================================================================
// Class Implementation
// =============================================================================
template <typename arg1_t, typename arg2_t, typename...arg_other_ts>
CommandBase::RepresentationVector &CommandBase::buildRepresentationVector
(
	RepresentationVector &vec
)
{
	vec.push_back
	(
		static_cast
		<
			std::remove_reference
			<
				decltype(vec)
			>::type::value_type
		>
		(
			typeToRepresentation<arg1_t>()
		)
	);
	buildRepresentationVector<arg2_t, arg_other_ts...>(vec);
	return vec;
}

template <typename arg_t>
CommandBase::RepresentationVector &CommandBase::buildRepresentationVector
(
	RepresentationVector &vec
)
{
	vec.push_back
	(
		static_cast
		<
			std::remove_reference
			<
				decltype(vec)
			>::type::value_type
		>
		(
			typeToRepresentation<arg_t>()
		)
	);
	return vec;
}
} // end namespace dls
#endif /* end of include guard: COMMAND_BASE_TPP_VNPJCDBU */
