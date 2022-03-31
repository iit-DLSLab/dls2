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
// =============================================================================
// Includes
// =============================================================================
#include "dls2/command/command_base.hpp"

namespace dls
{
	// =============================================================================
	// Class Implementation
	// =============================================================================


	std::string CommandBase::getCommandName(){
		return name;
	}

	std::string CommandBase::getCommandOwner(){
		return owner;
	}

	std::string CommandBase::getCommandDoc(){
		return docstring;
	}

	template <>
	CommandBase::ArgumentType CommandBase::typeToRepresentation<ARGVOID>()
	{
		return ArgumentType::VOID;
	}

	template <>
	CommandBase::ArgumentType CommandBase::typeToRepresentation<void>()
	{
		return ArgumentType::VOID;
	}

	template <>
	CommandBase::ArgumentType CommandBase::typeToRepresentation<char>()
	{
		return ArgumentType::CHAR;
	}

	template <>
	CommandBase::ArgumentType CommandBase::typeToRepresentation<uint8_t>()
	{
		return ArgumentType::UINT8;
	}

	template <>
	CommandBase::ArgumentType CommandBase::typeToRepresentation<int16_t>()
	{
		return ArgumentType::INT16;
	}

	template <>
	CommandBase::ArgumentType CommandBase::typeToRepresentation<uint16_t>()
	{
		return ArgumentType::UINT16;
	}

	template <>
	CommandBase::ArgumentType CommandBase::typeToRepresentation<int32_t>()
	{
		return ArgumentType::INT32;
	}

	template <>
	CommandBase::ArgumentType CommandBase::typeToRepresentation<int64_t>()
	{
		return ArgumentType::INT64;
	}

	template <>
	CommandBase::ArgumentType CommandBase::typeToRepresentation<uint64_t>()
	{
		return ArgumentType::UINT64;
	}

	template <>
	CommandBase::ArgumentType CommandBase::typeToRepresentation<float>()
	{
		return ArgumentType::FLOAT;
	}

	template <>
	CommandBase::ArgumentType CommandBase::typeToRepresentation<double>()
	{
		return ArgumentType::DOUBLE;
	}

	template <>
	CommandBase::ArgumentType CommandBase::typeToRepresentation<long double>()
	{
		return ArgumentType::LONG_DOUBLE;
	}

	template <>
	CommandBase::ArgumentType CommandBase::typeToRepresentation<bool>()
	{
		return ArgumentType::BOOL;
	}

	template <>
	CommandBase::ArgumentType CommandBase::typeToRepresentation<std::string>()
	{
		return ArgumentType::STD_STRING;
	}

	template <>
	CommandBase::ArgumentType CommandBase::typeToRepresentation<const std::string&>()
	{
		return ArgumentType::STD_STRING;
	}

	template <>
	CommandBase::ArgumentType CommandBase::typeToRepresentation<std::string&>()
	{
		return ArgumentType::STD_STRING;
	}

} // end namespace dls

