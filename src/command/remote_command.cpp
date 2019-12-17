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
	remote_command_publisher(msg)
{
	std::cout << "built remote command" << std::endl;
}

// -----------------------------------------------------------------------------
// Implementation
// -----------------------------------------------------------------------------
void RemoteCommand::clearArgs()
{
	// TODO implement
}

void RemoteCommand::call()
{
	// TODO implement
}

// -----------------------------------------------------------------------------
// Helper Classes
// -----------------------------------------------------------------------------
// =================== Remote Command Publisher Constructors ===================
RemoteCommand::RemoteCommandPublisher::RemoteCommandPublisher
(
	const CommandRegisterMsg &//msg
) :
	dynamic_type(),
	pData(nullptr),
	pParticipant(nullptr),
	pPublisher(nullptr)
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
	struct_type_builder->add_member
	(
		0,
		"index",
		eprosima::fastrtps::types::DynamicTypeBuilderFactory::get_instance()->
			create_uint32_type()
	);

	struct_type_builder->add_member
	(
		1,
		"message",
		eprosima::fastrtps::types::DynamicTypeBuilderFactory::get_instance()->
			create_string_type()
	);

	struct_type_builder->set_name("HelloWorld");

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
	pData->set_uint32_value(0, 0);
	pData->set_string_value("HelloWorld", 1);

	// create participant
	eprosima::fastrtps::ParticipantAttributes participant_attributes;
	participant_attributes.rtps.builtin.domainId = 0;
	participant_attributes.rtps.setName("DynHelloWorld_pub");

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
	publisher_attributes.topic.topicDataType = "HelloWorld";
	publisher_attributes.topic.topicName = "HelloWorldTopic";
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
	std::this_thread::sleep_for(std::chrono::milliseconds(80));
	for(size_t i = 0; i != 5; ++i)
	{
		std::cout << "sending dummy data" << std::endl;
		pData->set_uint32_value(i, 0);
		this->pPublisher->write((void*)this->pData.get());
		std::this_thread::sleep_for(std::chrono::seconds(1));
	}

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
{ }

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
