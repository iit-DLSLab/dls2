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
#ifndef COMMAND_HPP_RSTXNA3I
#define COMMAND_HPP_RSTXNA3I

// =============================================================================
// Includes
// =============================================================================
#include "dls2/command/command_base.hpp"
#include "dls2/util/messaging/publisher_base.hpp"
#include "dls2/util/messaging/subscriber_base.hpp"
#include "dls2/msg/command_registerPubSubTypes.h"
#include <string>
#include <functional>
#include <utility>
#include <vector>
#include <memory>
#include <fastrtps/types/DynamicTypeBuilderPtr.h>
#include <fastrtps/types/DynamicPubSubType.h>
#include "dls2/domains/domains.hpp"

namespace dls
{
// =============================================================================
// Class Interface
// =============================================================================
/// Template class representing a local command
///
/// This class is used to register a command with the framework. Users should
/// prefer using CommandManager instead of using Command directly
template <typename ret_t, typename...arg_ts>
class Command : public CommandBase
{
public:
	/// Constructor
	///
	/// @param owner the component that registered this command
	/// @param command_name the name of the command as seen from the rest of the
	/// framework
	/// @param docstring some documentation for the command
	/// @param f the function associated with the command. The function may take
	/// any number of any type of argument, and returns any type
	Command
	(
		const std::string &owner,
		const std::string &command_name,
		const std::string &docstring,
		const std::function<ret_t(arg_ts...)> &f
	);

	/// Destructor
	///
	/// Request removal of the command from the framework
	~Command();

	/// Registers this command with the framework
	///
	//void requestRegistration() override;

	/// Removes this command from the framework
	///
	//void requestDeregistration() override;
	
private:
	// ========================== Constructor helpers ==========================
	/// Constructor helper
	///
	/// Builds the CommandRegisterMsg used to advertise this command
	CommandRegisterMsg buildMsg
	(
		const std::string &owner,
		const std::string &command_name,
		const std::string &docstring
	);

	// =========================== Member Functions ============================
	ret_t call(std::tuple<arg_ts...>&);

	template <size_t...I>
	ret_t call(std::tuple<arg_ts...>&, std::index_sequence<I...>);

	// ============================= Data Members ==============================
	/// Name of the component that owns this command
	///
	const std::string owner;

	/// Name of the command as seen by the rest of the framework
	///
	const std::string command_name;

	/// Documentation for the command
	///
	const std::string docstring;

	/// Callback of the command
	///
	const std::function<ret_t(arg_ts...)> f;

	/// Registration message of the command
	///
	CommandRegisterMsg msg;

	/// Subscriber used to link the command with the rest of the framework
	///
	version2::Subscriber<CommandRegisterMsgPubSubType> commandSub;
};


// ============================ Subscriber reading =============================
// TODO put these into a proper namespace
/// Takes an argument from a dynamic subscriber
///
template <typename T>
T takeArg
(
	std::shared_ptr<eprosima::fastrtps::types::DynamicData> pData,
	size_t index
);

} // end namespace dls

#include "dls2/command/command.tpp"

#endif /* end of include guard: COMMAND_HPP_RSTXNA3I */
