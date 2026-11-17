#ifndef COMMAND_CPP
#define COMMAND_CPP

#include "dls2/command/command_without_args.hpp"
#include "dls2/command/command_manager.hpp"

namespace dls
{
	CommandWithoutArgs::CommandWithoutArgs
	(
		std::string name_,
		CommandManager *owner_,
		std::string docstring_,
		const std::function<bool()> &f_,
		dls::CommandBase::LevelType level_,
		bool enabled_
	) :
		CommandBase(
			name_,
			owner_,
			docstring_,
			0,
			level_,
			enabled_
		),
		f(f_)
	{ }

	
	CommandWithoutArgs::~CommandWithoutArgs(){}


	void CommandWithoutArgs::setEnabled()
	{
		if(this->isEnabled())
			return;
		this->enabled = true;
	}

	
	void CommandWithoutArgs::setDisabled()
	{
		if(this->isEnabled())
			return;
		this->enabled = false;
	}

	
	void CommandWithoutArgs::activate()
	{
		if(!this->isEnabled() || this->isActive())
			return;

		// the calls are received by the single reader of the CommandManager
		this->active = true;
	}

	
	void CommandWithoutArgs::deactivate()
	{
		if(!this->isEnabled() || !this->isActive())
			return;

		this->active = false;
	}

	
	bool CommandWithoutArgs::call()
	{
		// // ensure args are correct size

		// if (args.size() != this->getNumArgs()){
		// 	std::cerr << "Error: incorrect number of arguments" << std::endl;
		// 	return false;
		// }

		// auto arguments = create_tuple(args);
		// //static_assert(std::is_same_v<decltype(arguments), const std::tuple>);
			
		if(std::apply(this->f, std::make_tuple()))
            CommandHelper::changeCommandLevel(this->owner, this->getNextLevel(CommandHelper::getCurrentLevel(this->owner)));

		return true;
	}

	
	bool CommandWithoutArgs::call(std::vector<std::string>)
	{
		return this->call();
	}


	// -----------------------------------------------------------------------------
	// Class Helpers
	// -----------------------------------------------------------------------------
	// template<class T> T transform_args(std::string const &s);

	// template <typename... Args, std::size_t... Is>
	// auto create_tuple_impl(std::index_sequence<Is...>, const std::vector<std::string>& arguments)
	// {
	// 	return std::make_tuple(transform_args<Args>(arguments[Is])...);
	// }

	// template <typename... Args>
	// auto create_tuple(const std::vector<std::string>& args)
	// {
	// 	return create_tuple_impl<Args...>(std::index_sequence_for<Args...>{}, args);
	// }
    
    // template<> double transform_args<double>(std::string const &s) { return atof(s.c_str());}
    // template<> int transform_args<int>(std::string const &s) { return atoi(s.c_str());}
    // template<> bool transform_args<bool>(std::string const &s) { return (s != "0");}
    // template<> std::string transform_args<std::string>(std::string const &s) { return s;}
} // end namespace dls

#endif /* end of include guard: COMMAND_CPP */