#ifndef COMMAND_CPP
#define COMMAND_CPP

#include "dls2/command/command_new.hpp"
#include "dls2/command/command_manager.hpp"

namespace dls
{
	CommandNew::CommandNew
	(
		std::string name_,
		CommandManager *owner_,
		std::string docstring_,
		std::function<bool(void)> f_,
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
		f(f_),
		ddslink(nullptr)
	{ }

	
	CommandNew::~CommandNew(){}


	// void CommandNew::setEnabled()
	// {
	// 	if(this->isEnabled())
	// 		return;

	// 	this->registerCommand();
	// 	this->enabled = true;
	// }

	
	// void CommandNew::setDisabled()
	// {
	// 	if(this->isEnabled())
	// 		return;

	// 	this->unregisterCommand();
	// 	this->enabled = false;
	// }

	
	void CommandNew::activate()
	{
		if(!this->isEnabled() || this->isActive())
			return;

		this->registerCommand();
		this->active = true;
	}

	
	void CommandNew::deactivate()
	{
		if(!this->isEnabled() || !this->isActive() || (this->ddslink == nullptr)) 
			return;

		this->ddslink.reset();
		this->active = false;
	}

	
	bool CommandNew::call()
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

	
	void CommandNew::registerCommand()
	{
		if (ddslink != nullptr)
			return;
		// Define QoS for the data reader
		eprosima::fastdds::dds::DataReaderQos qos(eprosima::fastdds::dds::DATAREADER_QOS_DEFAULT);
		qos.reliability().kind = eprosima::fastdds::dds::RELIABLE_RELIABILITY_QOS;

		// Create the data reader
		ddslink = std::make_shared<dls::DDSReader>(
			this->getOwner() + "::" + this->getName(),
			dls::domains::command,
			topics::command_call,
			std::function<void(void *)>
			{
				[&](void *tuple)
				{
					CommandCallMsg* msg = (CommandCallMsg *)tuple;
					// if the owner is known but it is not the one owning this command OR
					// if the received command name is not equal to the name of this command
					// do nothing
					if ((msg->owner() != "" && msg->owner() != this->getOwner()) || msg->command_name() != this->getName())
						return;

					// std::vector<std::string> result;

					// //parse args
					// size_t pos = 0;
					// while ((pos = msg->args().find(",")) != std::string::npos) {
					// 	result.push_back( msg->args().substr(0, pos) );
					// 	msg->args().erase(0, pos + 1);
					// }			

					this->call();
				}
			},
			qos
		);
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