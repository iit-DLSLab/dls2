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
#ifndef COMMAND_BASE_HPP_BH05RHMM
#define COMMAND_BASE_HPP_BH05RHMM

// =============================================================================
// Includes
// =============================================================================
#include "dls2/msg/command_registerPubSubTypes.h"

// =============================================================================
// Class Interface
// =============================================================================
namespace dls
{
	/// Base class for command types
	///
	class CommandBase
	{
	public:
		/// Argument and return type representations
		///
		/// Since commands may be registered dynamically from external processes, we
		/// cannot simply use template types to instantiate new external commands.
		/// As such, the currently supported command types and arguments are
		/// encapsulated by the following enumeration
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

		CommandBase(
			std::string name_,
			std::string owner_,
			std::string doc_,
			uint nArg_
		);

		virtual ~CommandBase() = default;

		virtual int call(std::vector<std::string>);

		std::string getCommandName();

		std::string getCommandOwner();

		std::string getCommandDoc();

		uint getNumArgs();

	protected:
		/// Convenience typedef
		///
		typedef decltype(std::declval<CommandRegisterMsg>().arg_types())
			RepresentationVector;

		/// Converts a type to its representation for serialization and publishing
		///
		template <typename T>
		ArgumentType typeToRepresentation();

		/// Builds a vector of ArgumentTypes representing the types given in the
		/// temlate paramters
		///
		template <typename arg1_t, typename arg2_t, typename...arg_other_ts>
		RepresentationVector &buildRepresentationVector(RepresentationVector&);

		/// Builds a vector of ArgumentTypes representing the types given in the
		/// temlate paramters
		///
		/// Recursion base case
		template <typename arg_t>
		RepresentationVector &buildRepresentationVector(RepresentationVector&);
		

		/// Register this command with the framework
		///
		//virtual void requestRegistration() = 0;

		/// Unregister this command with the framework
		///
		//virtual void requestDeregistration() = 0;


	private:

		/// The name of the component that owns the remote command
		///
		std::string owner;

		/// The name of the command
		///
		std::string name;

		/// Command documentation
		///
		std::string docstring;

		/// Number of arguments of command
		///
		uint numArg;

		/// Callback of the command
		///
		const std::function<int(std::vector<std::string>)> f;
	};
} // end namespace dls

#include "dls2/command/command_base.tpp"
#endif /* end of include guard: COMMAND_BASE_HPP_BH05RHMM */
