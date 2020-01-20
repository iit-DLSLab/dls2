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
	void requestRegistration() override;

	/// Removes this command from the framework
	///
	void requestDeregistration() override;

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

	/// Publisher used to register the command with the rest of the framework
	///
	PublisherBase<CommandRegisterMsgPubSubType> publisher;

	/// Subscriber listening for requests to call this command
	///
	class CommandCallListener : public eprosima::fastrtps::SubscriberListener
	{
	public:
		CommandCallListener(Command<ret_t, arg_ts...>&);

	private:
		// ======================== Interface Override =========================
		void onNewDataMessage
		(
			eprosima::fastrtps::Subscriber*
		) override;

		// =========================== Data Members ============================
		/// A reference to the command object that owns this listener
		///
		Command<ret_t, arg_ts...> &owner;

		/// The fastrtps participant
		///
		std::shared_ptr<eprosima::fastrtps::Participant> pParticipant;

		/// The fastrtps subscriber
		///
		std::shared_ptr<eprosima::fastrtps::Subscriber> pSubscriber;

		/// The dynamic type
		///
		eprosima::fastrtps::types::DynamicPubSubType dynamic_type;

		/// Data storage
		///
		/// When a new message is received, it is stored here
		std::shared_ptr<eprosima::fastrtps::types::DynamicData> pData;

		/// Build a tuple from the arguments from a received message
		///
		/// Takes data stored in `pData` and recursively builds a tuple from it
		template <typename tuple_arg1_t, typename tuple_arg2_t, typename... tuple_arg_ts>
		std::tuple<tuple_arg1_t, tuple_arg2_t, tuple_arg_ts...> buildArgTuple
		(
			size_t index
		);

		/// Build a tuple from the arguments from a received message
		///
		/// recursion base case
		template <typename tuple_arg_t>
		std::tuple<tuple_arg_t> buildArgTuple
		(
			size_t index
		);

	} command_call_listener;
};
// =============================================================================
// Free Functions
// =============================================================================
// -----------------------------------------------------------------------------
// CommandCallListener
// -----------------------------------------------------------------------------
// ======================== Constructor Helpers ========================
// TODO put these into a proper namespace

/// Build the dynamic type of a dynamic subscriber
///
/// Recursively adds argument types to the subscriber
template <typename arg1_t, typename arg2_t, typename... arg_other_ts>
void buildDynamicType
(
	eprosima::fastrtps::types::DynamicTypeBuilder_ptr&,
	size_t index = 0
);

/// Build the dynamic type of a dynamic subscriber
///
/// Recursion base case
template <typename arg>
void buildDynamicType
(
	eprosima::fastrtps::types::DynamicTypeBuilder_ptr&,
	size_t index = 0
);

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

// =============================================================================
// Container Class
// =============================================================================
/// Command Manager class
///
/// External components should prefer using this class instead of directly using
/// `Command`
class CommandManager
{
public:
	/// Constructor
	///
	CommandManager();

	/// Destructor
	///
	~CommandManager();


	/// Adds a command to the CommandManager and registers it with the rest of
	/// the framework
	///
	/// @param owner name of the component that owns the command
	/// @command_name command name as seen by the rest of the framework
	/// @param docstring some documentation for the command
	/// @param f the function encapsulated by the command
	template <typename ret_t, typename... arg_ts>
	void addCommand
	(
		const std::string &owner,
		const std::string &command_name,
		const std::string &docstring,
		const std::function<ret_t(arg_ts...)> &f
	);

private:
	/// Storage space for the commands
	///
	std::vector<std::unique_ptr<CommandBase>> commands;
};
} // end namespace dls

#include "dls2/command/command.tpp"

#endif /* end of include guard: COMMAND_HPP_RSTXNA3I */
