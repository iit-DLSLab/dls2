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
#ifndef COMMAND_BASE_HPP_BH05RHMM
#define COMMAND_BASE_HPP_BH05RHMM

// =============================================================================
// Includes
// =============================================================================
#include "msg/command_registerPubSubTypes.h"

// =============================================================================
// Class Interface
// =============================================================================
namespace dls
{
class CommandBase
{
public:
	enum class ArgumentType : uint32_t
	{
		VOID,
		CHAR,
		UINT8,
		INT16,
		UINT16,
		INT32,
		UINT32,
		INT64,
		UINT64,
		FLOAT,
		DOUBLE,
		LONG_DOUBLE,
		BOOL,
		STD_STRING
	};

protected:
	typedef decltype(std::declval<CommandRegisterMsg>().arg_types())
		RepresentationVector;

	template <typename T>
	ArgumentType typeToRepresentation();

	template <typename arg1_t, typename arg2_t, typename...arg_other_ts>
	RepresentationVector &buildRepresentationVector(RepresentationVector&);

	template <typename arg_t>
	RepresentationVector &buildRepresentationVector(RepresentationVector&);
};
} // end namespace dls

#include "command/command_base.tpp"
#endif /* end of include guard: COMMAND_BASE_HPP_BH05RHMM */
