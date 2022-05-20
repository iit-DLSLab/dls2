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
	//foward declaration of CommandManager class
	class CommandManager;
	/// Base class for command types
	///
	class CommandBase
	{
	friend class CommandManager;
	public:
		typedef std::map<uint, uint> LevelType;

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
			CommandManager *owner,
			std::string doc,
			uint nArg,
			LevelType level = {{0,0}},
			bool enabled = false
		);

		/// Destructor
		///
		virtual ~CommandBase() = default;

		/// Retrieves command name string
		///
		std::string getName();

		/// Sets command name
		///
		void setName(std::string name_);		

		/// Retrieves command owner's name string
		///
		std::string getOwner();

		/// Retrieves command documentation string
		///
		std::string getDoc();

		/// Sets command documentation
		///
		void setDoc(std::string doc_);

		/// Retrieves command number of arguments
		///
		uint getNumArgs();

		/// Retrieve the current state of the command
		///
		bool isEnabled();

		/// Activate the command
		///
		virtual void activate();

		/// Deactivate the command
		///
		virtual void deactivate();

		/// Retrieve the current active state of the command
		///
		bool isActive();

		int getNextLevel(uint curLevel);
		bool testLevel(uint level);

	protected:

		/// Command execution level
		///
		LevelType transitionSet;

		/// If the command is enabled or not
		///
		bool enabled;

		/// If the command is active or not
		///
		bool active;

		/// The command_manager that owns the command
		///
		CommandManager *owner;
		
	private:

		/// The name of the command
		///
		std::string name;

		/// Command documentation
		///
		std::string doc;

		/// Number of arguments of command
		///
		uint numArg;

		/// Associated function call of the command
		///
		virtual int call(std::vector<std::string>);

	};




} // end namespace dls
#endif /* end of include guard: COMMAND_BASE_HPP_BH05RHMM */
