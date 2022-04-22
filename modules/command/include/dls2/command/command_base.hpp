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
#include "dls2/topics/topics.hpp"


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

		/// Constructor
		///
		/// @param name the name of the command as seen from the rest of the
		/// framework
		/// @param owner the component that registered this command
		/// @param doc some documentation for the command
		/// @param nArg number of arguments of the command
		/// @param level execution level of the command
		/// @param enabled set if the command is enabled or not
		CommandBase(
			std::string name,
			std::string owner,
			std::string doc,
			uint nArg,
			uint level = 0,
			bool enabled = false
		);

		/// Destructor
		///
		virtual ~CommandBase() = default;

		/// Associated function call of the command
		///
		virtual int call(std::vector<std::string>);

		/// Retrieves command name string
		///
		std::string getName();

		/// Sets command name
		///
		void setName(std::string name_);		

		/// Retrieves command owner's name string
		///
		std::string getOwner();

		/// Sets command owner
		///
		void setOwner(std::string owner_);				

		/// Retrieves command documentation string
		///
		std::string getDoc();

		/// Sets command documentation
		///
		void setDoc(std::string doc_);

		/// Retrieves command execution level
		///
		uint getLevel();

		/// Sets command execution level
		///
		void setLevel(uint level_);

		/// Retrieves command number of arguments
		///
		uint getNumArgs();

		/// Enable the command
		/// 
		void setEnabled();

		/// Disable the command
		///
		void setDisabled();

		/// Retrieve the current state of the command
		///
		bool isEnabled();

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
		

	private:

		/// The name of the component that owns the remote command
		///
		std::string owner;

		/// The name of the command
		///
		std::string name;

		/// Command documentation
		///
		std::string doc;

		/// Number of arguments of command
		///
		uint numArg;

		/// Command execution level
		///
		uint level;

		/// If the command is enabled or not
		///
		bool enabled;
	};
} // end namespace dls

#include "dls2/command/command_base.tpp"
#endif /* end of include guard: COMMAND_BASE_HPP_BH05RHMM */
