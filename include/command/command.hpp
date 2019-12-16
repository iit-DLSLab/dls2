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
#ifndef COMMAND_HPP_RSTXNA3I
#define COMMAND_HPP_RSTXNA3I

// =============================================================================
// Includes
// =============================================================================
#include "util/messaging/publisher_base.hpp"
#include "msg/command_registerPubSubTypes.h"
#include <string>
#include <functional>
#include <utility>
#include <vector>
#include <memory>

namespace dls
{
// =============================================================================
// Base Class
// =============================================================================
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
	typedef decltype(std::declval<CommandRegisterMsg>().arg_types()) RepresentationVector;

	template <typename T>
	ArgumentType typeToRepresentation();

	template <typename arg1_t, typename arg2_t, typename...arg_other_ts>
	RepresentationVector &buildRepresentationVector(RepresentationVector&);

	template <typename arg_t>
	RepresentationVector &buildRepresentationVector(RepresentationVector&);
};

// =============================================================================
// Class Interface
// =============================================================================
template <typename ret_t, typename...arg_ts>
class Command : public CommandBase
{
public:
	Command
	(
		const std::string &owner,
		const std::string &command_name,
		const std::string &docstring,
		const std::function<ret_t(arg_ts...)> &f
	);

	void requestRegistration();
	void requestDeregistration();

private:
	// ========================== Constructor helpers ==========================
	CommandRegisterMsg buildMsg
	(
		const std::string &owner,
		const std::string &command_name,
		const std::string &docstring
	);

	// ============================= Data Members ==============================
	const std::string owner;
	const std::string command_name;
	const std::string docstring;
	const std::function<ret_t(arg_ts...)> f;
	CommandRegisterMsg msg;
	PublisherBase<CommandRegisterMsgPubSubType> publisher;
};

// =============================================================================
// Container Class
// =============================================================================
class CommandManager
{
public:
	CommandManager();
	template <typename ret_t, typename... arg_ts>
	void addCommand
	(
		const std::string &owner,
		const std::string &command_name,
		const std::string &docstring,
		const std::function<ret_t(arg_ts...)> &f
	);

private:
	std::vector<std::unique_ptr<CommandBase>> commands;
};
} // end namespace dls

#include "command/command.tpp"

#endif /* end of include guard: COMMAND_HPP_RSTXNA3I */
