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
// =============================================================================
// Includes
// =============================================================================
#include <iostream>
#include <vector>

#include "dls2/topics/command_register.hpp"
#include "dls2/command/remote_command.hpp"
#include "dls2/topics/command_call.hpp"

#include <fastrtps/types/DynamicTypeBuilderFactory.h>
#include <fastrtps/attributes/PublisherAttributes.h>
#include <fastrtps/types/DynamicTypeBuilderPtr.h>
#include <fastrtps/types/DynamicTypeBuilder.h>
#include <fastrtps/types/DynamicDataFactory.h>
#include <fastrtps/publisher/Publisher.h>
#include <fastrtps/types/DynamicType.h>
#include <fastrtps/types/DynamicData.h>
#include <fastrtps/Domain.h>

// =============================================================================
// Using Declarations
// =============================================================================
using namespace dls;

// =============================================================================
// Remote Command
// =============================================================================
// -----------------------------------------------------------------------------
// Constructors
// -----------------------------------------------------------------------------
RemoteCommand::RemoteCommand(CommandRegisterMsg &msg) :
	owner(msg.owner()),
	command_name(msg.command_name()),
	docstring(msg.docstring()),
	args(msg.arg_types()),
	ret_type(msg.ret_type()),
	remote_command_publisher(*this, msg)
{ }

// -----------------------------------------------------------------------------
// Implementation
// -----------------------------------------------------------------------------
void RemoteCommand::clearArgs() const
{
	// TODO implement
	this->remote_command_publisher.command_arg_index = 0;
}

void RemoteCommand::call() const
{
	this->remote_command_publisher.pPublisher->write
	(
		(void*)this->remote_command_publisher.pData.get()
	);
	this->clearArgs();
}

// -----------------------------------------------------------------------------
// Helper Classes
// -----------------------------------------------------------------------------
// =================== Remote Command Publisher Constructors ===================
RemoteCommand::RemoteCommandPublisher::RemoteCommandPublisher
(
	RemoteCommand &owner_,
	const CommandRegisterMsg &msg
) :
	dynamic_type(),
	pData(nullptr),
	pParticipant(nullptr),
	pPublisher(nullptr),
	command_arg_index(0)
{
	// create a builder
	eprosima::fastrtps::types::DynamicTypeBuilder_ptr
		struct_type_builder
		(
			eprosima::fastrtps::types::DynamicTypeBuilderFactory::get_instance()->
				create_struct_builder()
		);

	// add members to the builder
	for(size_t i = 0; i != msg.arg_types().size(); ++i)
	{
		eprosima::fastrtps::types::DynamicType_ptr pType;
		auto pFactory = eprosima::fastrtps::types::DynamicTypeBuilderFactory::get_instance();

		// Using a lambda because for some reasong this code won't work when
		// just copying the contents of the lambda into its single call site at
		// the end of this switch statement
		auto add_member = [&]()
		{
			struct_type_builder->add_member
			(
				i,
				std::string("field_") + std::to_string(i),
				pType
			);
		};
		switch(static_cast<CommandBase::ArgumentType>(msg.arg_types()[i]))
		{
			case CommandBase::ArgumentType::VOID:
				continue;
			case CommandBase::ArgumentType::CHAR:
				pType = pFactory->create_char8_type();
				break;
			case CommandBase::ArgumentType::UINT8:
				pType = pFactory->create_uint16_type();
				break;
			case CommandBase::ArgumentType::INT16:
				pType = pFactory->create_int16_type();
				break;
			case CommandBase::ArgumentType::UINT16:
				pType = pFactory->create_uint16_type();
				break;
			case CommandBase::ArgumentType::INT32:
				pType = pFactory->create_int32_type();
				break;
			case CommandBase::ArgumentType::UINT32:
				pType = pFactory->create_uint32_type();
				break;
			case CommandBase::ArgumentType::INT64:
				pType = pFactory->create_int64_type();
				break;
			case CommandBase::ArgumentType::UINT64:
				pType = pFactory->create_uint64_type();
				break;
			case CommandBase::ArgumentType::FLOAT:
				pType = pFactory->create_float32_type();
				break;
			case CommandBase::ArgumentType::DOUBLE:
				pType = pFactory->create_float64_type();
				break;
			case CommandBase::ArgumentType::LONG_DOUBLE:
				pType = pFactory->create_float128_type();
				break;
			case CommandBase::ArgumentType::BOOL:
				break;
			case CommandBase::ArgumentType::STD_STRING:
				pType = pFactory->create_string_type();
				break;
			default:
				break;
		}
		add_member();
	}

	struct_type_builder->set_name
	(
		(
			std::string(topics::command_call) + "_" + owner_.owner + "_" +
			owner_.command_name + "_struct"
		).c_str()
	);

	// create a struct from the builder
	eprosima::fastrtps::types::DynamicType_ptr pDynamic_type =
		struct_type_builder->build();

	this->dynamic_type.SetDynamicType(pDynamic_type);
	this->pData.reset
	(
		eprosima::fastrtps::types::DynamicDataFactory::get_instance()->
			create_data(pDynamic_type),

		// TODO add proper deleter here
		[](eprosima::fastrtps::types::DynamicData*){}
	);
	// create participant
	eprosima::fastrtps::ParticipantAttributes participant_attributes;
	// participant_attributes.rtps.builtin.domainId = 0;
	participant_attributes.rtps.setName
	(
		(
			std::string(topics::command_call) + "_" + owner_.owner + "_" +
			owner_.command_name + "_pub_participant"
		).c_str()
	);

	// TODO add deleter
	this->pParticipant.reset
	(
		eprosima::fastrtps::Domain::createParticipant
		(
			participant_attributes
		),
		// TODO add prper deleter here
		[](eprosima::fastrtps::Participant*){}
	);
	if(!pParticipant)
	{
		// TODO do something more sensible here
		std::cout << "ERROR: dynamic participant was not created" << std::endl;
	}

	// Register the type
	eprosima::fastrtps::Domain::registerDynamicType
	(
		this->pParticipant.get(),
		&(this->dynamic_type)
	);

	// Create the publisher
	eprosima::fastrtps::PublisherAttributes publisher_attributes;
	publisher_attributes.topic.topicKind = eprosima::fastrtps::rtps::NO_KEY;
	publisher_attributes.topic.topicDataType = struct_type_builder->get_name();
	publisher_attributes.topic.topicName =
		std::string(topics::command_call) + "_" + owner_.owner + "_" +
		owner_.command_name;
	this->pPublisher.reset
	(
		eprosima::fastrtps::Domain::createPublisher
		(
			this->pParticipant.get(),
			publisher_attributes
		),
		// TODO add proper deleter here
		[](eprosima::fastrtps::Publisher*){}
	);

	if(!this->pPublisher)
	{
		// TODO do something better here
		std::cout << "ERROR: dynamic publisher was not created" << std::endl;
	}
}

// =============================================================================
// Remote Command Callable
// =============================================================================
// -----------------------------------------------------------------------------
// Constructors
// -----------------------------------------------------------------------------
RemoteCommandCallable::RemoteCommandCallable() :
	pRemote_command(nullptr)
{ }

// =============================================================================
// Remote Command Manager
// =============================================================================
// -----------------------------------------------------------------------------
// Constructors
// -----------------------------------------------------------------------------
RemoteCommandManager::RemoteCommandManager
(
	std::function<void(std::shared_ptr<const RemoteCommand>)> onNewCommand_,
	std::function<void(std::shared_ptr<const RemoteCommand>)> onRemoveCommand_
) :
	remote_commands_mutex(),
	remote_commands(),
	command_added(),
	registration_listener(*this),
	onNewCommand(onNewCommand_),
	onRemoveCommand(onRemoveCommand_)
{ }

// -----------------------------------------------------------------------------
// Implementation
// -----------------------------------------------------------------------------
std::vector<std::shared_ptr<const RemoteCommand>> RemoteCommandManager::findByOwner
(
	const std::string &owner
) const
{
	std::vector<std::shared_ptr<const RemoteCommand>> vec;
	{
		std::lock_guard<std::mutex> lock(this->remote_commands_mutex);
		for(const auto &el : this->remote_commands)
		{
			if(el->owner == owner)
			{
				vec.push_back(el);
			}
		}
	}
	return vec;
}

std::vector<std::shared_ptr<const RemoteCommand>> RemoteCommandManager::findByName
(
	const std::string &name
) const
{
	std::vector<std::shared_ptr<const RemoteCommand>> vec;
	{
		std::lock_guard<std::mutex> lock(this->remote_commands_mutex);
		for(const auto &el : this->remote_commands)
		{
			if(el->command_name == name)
			{
				vec.push_back(el);
			}
		}
	}
	return vec;
}

std::shared_ptr<const RemoteCommand> RemoteCommandManager::find
(
	const std::string &owner,
	const std::string &name
) const
{
	{
		std::lock_guard<std::mutex> lock(this->remote_commands_mutex);
		for(const auto &el : this->remote_commands)
		{
			if
			(
				el->owner == owner &&
				el->command_name == name
			)
			{
				return el;
			}
		}
	}
	return nullptr;
}

std::vector<std::shared_ptr<const RemoteCommand>>
	RemoteCommandManager::getCurrentlyRegisteredCommands()
{
	std::lock_guard<std::mutex> lock(this->remote_commands_mutex);
	return this->remote_commands;
}

std::set<std::string> RemoteCommandManager::getCurrentlyRegisteredOwners()
{
	std::lock_guard<std::mutex> lock(this->remote_commands_mutex);
	std::set<std::string> set;
	for(auto it = remote_commands.begin(); it != remote_commands.end(); ++it)
	{
		set.insert((*it)->owner);
	}

	return set;
}

void RemoteCommandManager::addCommand(std::shared_ptr<RemoteCommand> pCommand)
{
	std::lock_guard<std::mutex> lock(this->remote_commands_mutex);
	// check whether the command that is being registered has already been
	// registered. This can happen, for instance, if the user force quits a
	// layer. In that case, the layer will not be able to publish that it is
	// removing its commands. When the layer is restarted, it will try to
	// reregister all of its commands. Do not accept the duplicates.
	bool already_exists = false;


	// This loop is not the most effective way to do this, but it's the simplest
	// and this is not performance-critical code
	for(const auto &command : this->remote_commands)
	{
		if
		(
			command->owner        == pCommand->owner &&
			command->command_name == pCommand->command_name
		)
		{
			already_exists = true;
			break;
		}
	}
	if(!already_exists)
	{
		this->remote_commands.push_back(pCommand);
		this->command_added.notify_all();
	}

	if(this->onNewCommand)
	{
		this->onNewCommand(pCommand);
	}
}

void RemoteCommandManager::removeCommand(const CommandRegisterMsg &msg)
{
	std::lock_guard<std::mutex> lock(this->remote_commands_mutex);
	for(auto it = this->remote_commands.begin(); it != this->remote_commands.end(); ++it)
	{
		if
		(
			(*it)->owner == msg.owner() &&
			(*it)->command_name == msg.command_name()
		)
		{
			if(this->onRemoveCommand)
			{
				this->onRemoveCommand(*it);
			}
			this->remote_commands.erase(it);
			break;
		}
	}
}

RemoteCommandCallable RemoteCommandManager::makeCallable
(
	const std::string &owner,
	const std::string &name
) const
{
	std::shared_ptr<const RemoteCommand> pCommand;
	do
	{
		pCommand = this->find
		(
			owner,
			name
		);
		if(!pCommand)
		{
			std::cout << "Command '" << name << "' for '" << owner <<
				"' not yet registered. Blocking" << std::endl;
			std::unique_lock<std::mutex> lock(this->remote_commands_mutex);
			this->command_added.wait(lock);
		}
	}while(pCommand == nullptr);

	RemoteCommandCallable callable;
	callable.pRemote_command = pCommand;
	return callable;
}

// -----------------------------------------------------------------------------
// Subscriber Helper
// -----------------------------------------------------------------------------
// =============================== Constructors ================================
RemoteCommandManager::RegistrationListener::RegistrationListener
(
	RemoteCommandManager &owner_
) :
	SubscriberBase<CommandRegisterMsgPubSubType>(topics::command_register),
	owner(owner_)
{ }

// ============================== Implementation ===============================
void RemoteCommandManager::RegistrationListener::onNewDataMessage
(
	eprosima::fastrtps::Subscriber *sub
)
{
	eprosima::fastrtps::SampleInfo_t info;
	CommandRegisterMsg msg;
	if(sub->takeNextData(&msg, &info))
	{
		if(msg.register_nremove())
		{
			std::shared_ptr<RemoteCommand> pCommand =
				std::make_shared<RemoteCommand>
				(
					msg
				);
			owner.addCommand(pCommand);
		}
		else
		{
			owner.removeCommand(msg);
		}
	}
}

// ============================== Push Arguments ===============================
namespace dls
{
template <>
void RemoteCommand::pushArg(ARGVOID) const
{
	this->remote_command_publisher.command_arg_index++;
}
template <>
void RemoteCommand::pushArg(char c) const
{
	this->remote_command_publisher.pData->set_char8_value
	(
		c,
		this->remote_command_publisher.command_arg_index
	);

	this->remote_command_publisher.command_arg_index++;
}

template <>
void RemoteCommand::pushArg(uint8_t i) const
{
	this->remote_command_publisher.pData->set_uint8_value
	(
		i,
		this->remote_command_publisher.command_arg_index
	);

	this->remote_command_publisher.command_arg_index++;
}

template <>
void RemoteCommand::pushArg(int16_t i) const
{
	this->remote_command_publisher.pData->set_int16_value
	(
		i,
		this->remote_command_publisher.command_arg_index
	);

	this->remote_command_publisher.command_arg_index++;
}

template <>
void RemoteCommand::pushArg(uint16_t i) const
{
	this->remote_command_publisher.pData->set_uint16_value
	(
		i,
		this->remote_command_publisher.command_arg_index
	);

	this->remote_command_publisher.command_arg_index++;
}

template <>
void RemoteCommand::pushArg(int32_t i) const
{
	this->remote_command_publisher.pData->set_int32_value
	(
		i,
		this->remote_command_publisher.command_arg_index
	);

	this->remote_command_publisher.command_arg_index++;
}

template <>
void RemoteCommand::pushArg(uint32_t i) const
{
	this->remote_command_publisher.pData->set_uint32_value
	(
		i,
		this->remote_command_publisher.command_arg_index
	);

	this->remote_command_publisher.command_arg_index++;
}

template <>
void RemoteCommand::pushArg(int64_t i) const
{
	this->remote_command_publisher.pData->set_int64_value
	(
		i,
		this->remote_command_publisher.command_arg_index
	);

	this->remote_command_publisher.command_arg_index++;
}

template <>
void RemoteCommand::pushArg(float f) const
{
	this->remote_command_publisher.pData->set_float32_value
	(
		f,
		this->remote_command_publisher.command_arg_index
	);

	this->remote_command_publisher.command_arg_index++;
}

template <>
void RemoteCommand::pushArg(double d) const
{
	this->remote_command_publisher.pData->set_float64_value
	(
		d,
		this->remote_command_publisher.command_arg_index
	);

	this->remote_command_publisher.command_arg_index++;
}

template <>
void RemoteCommand::pushArg(long double ld) const
{
	this->remote_command_publisher.pData->set_float128_value
	(
		ld,
		this->remote_command_publisher.command_arg_index
	);

	this->remote_command_publisher.command_arg_index++;
}

template <>
void RemoteCommand::pushArg(bool b) const
{
	this->remote_command_publisher.pData->set_bool_value
	(
		b,
		this->remote_command_publisher.command_arg_index
	);

	this->remote_command_publisher.command_arg_index++;
}

template <>
void RemoteCommand::pushArg(/*const*/ std::string /*&*/s) const
{
	this->remote_command_publisher.pData->set_string_value
	(
		s,
		this->remote_command_publisher.command_arg_index
	);

	this->remote_command_publisher.command_arg_index++;
}
} // end namespace dls
