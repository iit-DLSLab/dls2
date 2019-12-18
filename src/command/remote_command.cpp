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
#include "command/remote_command.hpp"
#include "topics/command_register.hpp"
#include <vector>
#include "topics/command_call.hpp"

#include <fastrtps/Domain.h>
#include <fastrtps/types/DynamicData.h>
#include <fastrtps/types/DynamicTypeBuilderFactory.h>
#include <fastrtps/types/DynamicDataFactory.h>
#include <fastrtps/types/DynamicTypeBuilder.h>
#include <fastrtps/types/DynamicTypeBuilderPtr.h>
#include <fastrtps/types/DynamicType.h>
#include <fastrtps/publisher/Publisher.h>
#include <fastrtps/attributes/PublisherAttributes.h>

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
{
	std::cout << "built remote command" << std::endl;
}

// -----------------------------------------------------------------------------
// Implementation
// -----------------------------------------------------------------------------
void RemoteCommand::clearArgs()
{
	// TODO implement
	this->remote_command_publisher.command_arg_index = 0;
}

void RemoteCommand::call()
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
	std::cout << "remote command pub constructor enter" << std::endl;
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
				std::cout << "void -- skipping" << std::endl;
				continue;
			case CommandBase::ArgumentType::CHAR:
				std::cout << "char8 " << i << std::endl;
				pType = pFactory->create_char8_type();
				break;
			case CommandBase::ArgumentType::UINT8:
				std::cout << "uint8 " << i << std::endl;
				pType = pFactory->create_uint16_type();
				break;
			case CommandBase::ArgumentType::INT16:
				std::cout << "int16 " << i << std::endl;
				pType = pFactory->create_int16_type();
				break;
			case CommandBase::ArgumentType::UINT16:
				std::cout << "uint16 " << i << std::endl;
				pType = pFactory->create_uint16_type();
				break;
			case CommandBase::ArgumentType::INT32:
				std::cout << "int32 " << i << std::endl;
				pType = pFactory->create_int32_type();
				break;
			case CommandBase::ArgumentType::UINT32:
				std::cout << "uint32 " << i << std::endl;
				pType = pFactory->create_uint32_type();
				break;
			case CommandBase::ArgumentType::INT64:
				std::cout << "int64 " << i << std::endl;
				pType = pFactory->create_int64_type();
				break;
			case CommandBase::ArgumentType::UINT64:
				std::cout << "uint64 " << i << std::endl;
				pType = pFactory->create_uint64_type();
				break;
			case CommandBase::ArgumentType::FLOAT:
				std::cout << "float " << i << std::endl;
				pType = pFactory->create_float32_type();
				break;
			case CommandBase::ArgumentType::DOUBLE:
				std::cout << "double " << i << std::endl;
				pType = pFactory->create_float64_type();
				break;
			case CommandBase::ArgumentType::LONG_DOUBLE:
				std::cout << "long double " << i << std::endl;
				pType = pFactory->create_float128_type();
				break;
			case CommandBase::ArgumentType::BOOL:
				std::cout << "bool " << i << std::endl;
				break;
			case CommandBase::ArgumentType::STD_STRING:
				std::cout << "string " << i << std::endl;
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
	// pData->set_uint32_value(0, 0);
	// pData->set_string_value("HelloWorld", 1);

	// create participant
	eprosima::fastrtps::ParticipantAttributes participant_attributes;
	participant_attributes.rtps.builtin.domainId = 0;
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

	// Send dummy data
	// this->pData->set_uint32_value(123, 0);
	// this->pData->set_string_value("HelloWorld", 1);
	// std::this_thread::sleep_for(std::chrono::seconds(10));
	// std::this_thread::sleep_for(std::chrono::milliseconds(80));
	// for(size_t i = 0; i != 5; ++i)
	// {
	// 	std::cout << "sending dummy data" << std::endl;
	// 	// pData->set_uint32_value(i, 0);
	// 	this->pPublisher->write((void*)this->pData.get());
	// 	std::this_thread::sleep_for(std::chrono::seconds(1));
	// }

}

// =============================================================================
// Remote Command Manager
// =============================================================================
// -----------------------------------------------------------------------------
// Constructors
// -----------------------------------------------------------------------------
RemoteCommandManager::RemoteCommandManager
(
	std::function<void(std::shared_ptr<RemoteCommand>)> onNewCommand_,
	std::function<void(std::shared_ptr<RemoteCommand>)> onRemoveCommand_
) :
	remote_commands_mutex(),
	remote_commands(),
	registration_listener(*this),
	onNewCommand(onNewCommand_),
	onRemoveCommand(onRemoveCommand_)
{
	std::cout << "Remote Command Manager Constructed" << std::endl;
}

// -----------------------------------------------------------------------------
// Implementation
// -----------------------------------------------------------------------------
std::vector<std::shared_ptr<RemoteCommand>> RemoteCommandManager::findByOwner
(
	const std::string &owner
)
{
	std::vector<std::shared_ptr<RemoteCommand>> vec;
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

std::vector<std::shared_ptr<RemoteCommand>> RemoteCommandManager::findByName
(
	const std::string &name
)
{
	std::vector<std::shared_ptr<RemoteCommand>> vec;
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

std::shared_ptr<RemoteCommand> RemoteCommandManager::find
(
	const std::string &owner,
	const std::string &name
)
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

std::vector<std::shared_ptr<RemoteCommand>>
	RemoteCommandManager::getCurrentlyRegisteredCommands()
{
	std::lock_guard<std::mutex> lock(this->remote_commands_mutex);
	return this->remote_commands;
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
		std::cout << "remote command manager building new command" << std::endl;
		std::shared_ptr<RemoteCommand> pCommand =
			std::make_shared<RemoteCommand>
			(
				msg
			);

		{
			std::lock_guard<std::mutex> lock(owner.remote_commands_mutex);
			owner.remote_commands.push_back(pCommand);
		}

		if(owner.onNewCommand)
		{
			owner.onNewCommand(pCommand);
		}
	}
}

// =================================== call ====================================

// ============================== Push Arguments ===============================
namespace dls
{
template <>
void RemoteCommand::pushArg(char c)
{
	std::cout << "push char" << std::endl;
	this->remote_command_publisher.pData->set_char8_value
	(
		c,
		this->remote_command_publisher.command_arg_index
	);

	this->remote_command_publisher.command_arg_index++;
}

template <>
void RemoteCommand::pushArg(uint8_t i)
{
	std::cout << "push uint8" << std::endl;
	this->remote_command_publisher.pData->set_uint8_value
	(
		i,
		this->remote_command_publisher.command_arg_index
	);

	this->remote_command_publisher.command_arg_index++;
}

template <>
void RemoteCommand::pushArg(int16_t i)
{
	std::cout << "push int16" << std::endl;
	this->remote_command_publisher.pData->set_int16_value
	(
		i,
		this->remote_command_publisher.command_arg_index
	);

	this->remote_command_publisher.command_arg_index++;
}

template <>
void RemoteCommand::pushArg(uint16_t i)
{
	std::cout << "push uint16" << std::endl;
	this->remote_command_publisher.pData->set_uint16_value
	(
		i,
		this->remote_command_publisher.command_arg_index
	);

	this->remote_command_publisher.command_arg_index++;
}

template <>
void RemoteCommand::pushArg(int32_t i)
{
	std::cout << "push int32" << std::endl;
	this->remote_command_publisher.pData->set_int32_value
	(
		i,
		this->remote_command_publisher.command_arg_index
	);

	this->remote_command_publisher.command_arg_index++;
}

template <>
void RemoteCommand::pushArg(uint32_t i)
{
	std::cout << "push uint32" << std::endl;
	this->remote_command_publisher.pData->set_uint32_value
	(
		i,
		this->remote_command_publisher.command_arg_index
	);

	this->remote_command_publisher.command_arg_index++;
}

template <>
void RemoteCommand::pushArg(int64_t i)
{
	std::cout << "push int64" << std::endl;
	this->remote_command_publisher.pData->set_int64_value
	(
		i,
		this->remote_command_publisher.command_arg_index
	);

	this->remote_command_publisher.command_arg_index++;
}

template <>
void RemoteCommand::pushArg(float f)
{
	std::cout << "push float" << std::endl;
	this->remote_command_publisher.pData->set_float32_value
	(
		f,
		this->remote_command_publisher.command_arg_index
	);

	this->remote_command_publisher.command_arg_index++;
}

template <>
void RemoteCommand::pushArg(double d)
{
	std::cout << "push double" << std::endl;
	this->remote_command_publisher.pData->set_float64_value
	(
		d,
		this->remote_command_publisher.command_arg_index
	);

	this->remote_command_publisher.command_arg_index++;
}

template <>
void RemoteCommand::pushArg(long double ld)
{
	std::cout << "push long double" << std::endl;
	this->remote_command_publisher.pData->set_float128_value
	(
		ld,
		this->remote_command_publisher.command_arg_index
	);

	this->remote_command_publisher.command_arg_index++;
}

template <>
void RemoteCommand::pushArg(bool b)
{
	std::cout << "push bool" << std::endl;
	this->remote_command_publisher.pData->set_bool_value
	(
		b,
		this->remote_command_publisher.command_arg_index
	);

	this->remote_command_publisher.command_arg_index++;
}

template <>
void RemoteCommand::pushArg(const std::string &s)
{
	std::cout << "push string" << std::endl;
	this->remote_command_publisher.pData->set_string_value
	(
		s,
		this->remote_command_publisher.command_arg_index
	);

	this->remote_command_publisher.command_arg_index++;
}
} // end namespace dls
