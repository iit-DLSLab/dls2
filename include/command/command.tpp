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
#ifndef COMMAND_TPP_OVALZHX0
#define COMMAND_TPP_OVALZHX0

// =============================================================================
// Includes
// =============================================================================
#include "command/command.hpp"
#include "topics/command_register.hpp"
#include <fastrtps/types/DynamicTypeBuilderFactory.h>
#include <fastrtps/types/DynamicDataFactory.h>
#include <fastrtps/types/DynamicData.h>
#include <iostream>

namespace dls
{
// =============================================================================
// Constructors
// =============================================================================
template <typename ret_t, typename...arg_ts>
Command<ret_t, arg_ts...>::Command
(
	const std::string &owner_,
	const std::string &command_name_,
	const std::string &docstring_,
	const std::function<ret_t(arg_ts...)>&f_
) :
	owner(owner_),
	command_name(command_name_),
	docstring(docstring_),
	f(f_),
	msg(buildMsg(owner_, command_name_, docstring_)),
	publisher(topics::command_register),
	command_call_listener(*this)
{
	requestRegistration();
}

// -----------------------------------------------------------------------------
// Constructor Helpers
// -----------------------------------------------------------------------------
template <typename ret_t, typename...arg_ts>
CommandRegisterMsg Command<ret_t, arg_ts...>::buildMsg
(
	const std::string &owner,
	const std::string &command_name,
	const std::string &docstring
)
{
	CommandRegisterMsg msg;
	msg.owner(owner);
	msg.command_name(command_name);
	msg.docstring(docstring);

	buildRepresentationVector<arg_ts...>(msg.arg_types());
	msg.ret_type
	(
		static_cast
		<
			std::remove_reference
			<
				decltype(msg.ret_type())
			>::type
		>
		(
			CommandBase::typeToRepresentation<ret_t>()
		)
	);
	return msg;
}

// =============================================================================
// Implementation
// =============================================================================
template <typename ret_t, typename...arg_ts>
void Command<ret_t, arg_ts...>::requestRegistration()
{
	this->publisher.publish
	(
		this->msg
	);
}

template <typename ret_t, typename...arg_ts>
void Command<ret_t, arg_ts...>::requestDeregistration()
{
	// TODO implement
}

// =============================================================================
// Helper Classes
// =============================================================================
// -----------------------------------------------------------------------------
// Command Call Listener Constructors
// -----------------------------------------------------------------------------
template <typename ret_t, typename...arg_ts>
Command<ret_t, arg_ts...>::CommandCallListener::CommandCallListener
(
	Command<ret_t, arg_ts...> &owner_
) :
	owner(owner_),
	pParticipant(nullptr),
	pSubscriber(nullptr),
	dynamic_type(),
	pData(nullptr)
{
	eprosima::fastrtps::ParticipantAttributes participant_attributes;
	participant_attributes.rtps.builtin.domainId = 0;
	participant_attributes.rtps.setName("DynHelloworld_sub");

	pParticipant.reset
	(
		eprosima::fastrtps::Domain::createParticipant(participant_attributes),
		// TODO add proper deleter here
		[](eprosima::fastrtps::Participant*){}
	);

	if(!pParticipant)
	{
		// TODO do something better here
		std::cout << "ERROR:: could not create dynamic participant" << std::endl;
	}

	eprosima::fastrtps::types::DynamicTypeBuilder_ptr struct_type_builder =
		eprosima::fastrtps::types::DynamicTypeBuilderFactory::get_instance()->
			create_struct_builder();

	buildDynamicType<arg_ts...>(struct_type_builder);
	// struct_type_builder->add_member
	// (
	// 	0,
	// 	"index",
	// 	eprosima::fastrtps::types::DynamicTypeBuilderFactory::get_instance()->
	// 		create_uint32_builder()
	// );

	// struct_type_builder->add_member
	// (
	// 	1,
	// 	"message",
	// 	eprosima::fastrtps::types::DynamicTypeBuilderFactory::get_instance()->
	// 		create_string_type()
	// );
	struct_type_builder->set_name("HelloWorld");

	eprosima::fastrtps::types::DynamicType_ptr dtp = struct_type_builder->build();
	this->dynamic_type.SetDynamicType(dtp);
	this->pData.reset
	(
		eprosima::fastrtps::types::DynamicDataFactory::get_instance()->
			create_data(dtp),
		// TODO put proper deleter here
		[](eprosima::fastrtps::types::DynamicData*){}
	);

	eprosima::fastrtps::Domain::registerDynamicType
	(
		this->pParticipant.get(),
		&this->dynamic_type
	);

	eprosima::fastrtps::SubscriberAttributes subscriber_attributes;
	subscriber_attributes.topic.topicKind = eprosima::fastrtps::rtps::NO_KEY;
	subscriber_attributes.topic.topicDataType = "HelloWorld";
	subscriber_attributes.topic.topicName = "HelloWorldTopic";

	this->pSubscriber.reset
	(
		eprosima::fastrtps::Domain::createSubscriber
		(
			this->pParticipant.get(),
			subscriber_attributes,
			this
		),
		// TODO put proper deleter here
		[](eprosima::fastrtps::Subscriber*){}
	);

	if(!this->pSubscriber)
	{
		// TODO something more sensible here
		std::cout << "ERROR: could not create dynamic subscriber" << std::endl;
	}
	std::cout << "created remote call listener" << std::endl;
}

// -----------------------------------------------------------------------------
// Constructor Helpers
// -----------------------------------------------------------------------------
template <typename arg1_t, typename arg2_t, typename... arg_other_ts>
void buildDynamicType
(
	eprosima::fastrtps::types::DynamicTypeBuilder_ptr &builder,
	size_t index
)
{
	buildDynamicType<arg1_t>(builder, index);
	buildDynamicType<arg2_t, arg_other_ts...>(builder, ++index);
}

// ========================= Base Case Specialisations =========================
template<>
void buildDynamicType<char>
(
	eprosima::fastrtps::types::DynamicTypeBuilder_ptr &builder,
	size_t index
)
{
	builder->add_member
	(
		index,
		std::string("field_") + std::to_string(index),
		eprosima::fastrtps::types::DynamicTypeBuilderFactory::get_instance()->
			create_char8_builder()
	);
}

template<>
void buildDynamicType<uint8_t>
(
	eprosima::fastrtps::types::DynamicTypeBuilder_ptr &builder,
	size_t index
)
{
	builder->add_member
	(
		index,
		std::string("field_") + std::to_string(index),
		eprosima::fastrtps::types::DynamicTypeBuilderFactory::get_instance()->
			create_uint16_builder()
	);
}

template<>
void buildDynamicType<int16_t>
(
	eprosima::fastrtps::types::DynamicTypeBuilder_ptr &builder,
	size_t index
)
{
	builder->add_member
	(
		index,
		std::string("field_") + std::to_string(index),
		eprosima::fastrtps::types::DynamicTypeBuilderFactory::get_instance()->
			create_int16_builder()
	);
}

template<>
void buildDynamicType<uint16_t>
(
	eprosima::fastrtps::types::DynamicTypeBuilder_ptr &builder,
	size_t index
)
{
	builder->add_member
	(
		index,
		std::string("field_") + std::to_string(index),
		eprosima::fastrtps::types::DynamicTypeBuilderFactory::get_instance()->
			create_uint16_builder()
	);
}

template<>
void buildDynamicType<int32_t>
(
	eprosima::fastrtps::types::DynamicTypeBuilder_ptr &builder,
	size_t index
)
{
	builder->add_member
	(
		index,
		std::string("field_") + std::to_string(index),
		eprosima::fastrtps::types::DynamicTypeBuilderFactory::get_instance()->
			create_int32_builder()
	);
}

template<>
void buildDynamicType<uint32_t>
(
	eprosima::fastrtps::types::DynamicTypeBuilder_ptr &builder,
	size_t index
)
{
	builder->add_member
	(
		index,
		std::string("field_") + std::to_string(index),
		eprosima::fastrtps::types::DynamicTypeBuilderFactory::get_instance()->
			create_uint32_builder()
	);
}

template<>
void buildDynamicType<int64_t>
(
	eprosima::fastrtps::types::DynamicTypeBuilder_ptr &builder,
	size_t index
)
{
	builder->add_member
	(
		index,
		std::string("field_") + std::to_string(index),
		eprosima::fastrtps::types::DynamicTypeBuilderFactory::get_instance()->
			create_int64_builder()
	);
}

template<>
void buildDynamicType<uint64_t>
(
	eprosima::fastrtps::types::DynamicTypeBuilder_ptr &builder,
	size_t index
)
{
	builder->add_member
	(
		index,
		std::string("field_") + std::to_string(index),
		eprosima::fastrtps::types::DynamicTypeBuilderFactory::get_instance()->
			create_uint64_builder()
	);
}

template<>
void buildDynamicType<float>
(
	eprosima::fastrtps::types::DynamicTypeBuilder_ptr &builder,
	size_t index
)
{
	builder->add_member
	(
		index,
		std::string("field_") + std::to_string(index),
		eprosima::fastrtps::types::DynamicTypeBuilderFactory::get_instance()->
			create_float32_builder()
	);
}

template<>
void buildDynamicType<double>
(
	eprosima::fastrtps::types::DynamicTypeBuilder_ptr &builder,
	size_t index
)
{
	builder->add_member
	(
		index,
		std::string("field_") + std::to_string(index),
		eprosima::fastrtps::types::DynamicTypeBuilderFactory::get_instance()->
			create_float64_builder()
	);
}

template<>
void buildDynamicType<long double>
(
	eprosima::fastrtps::types::DynamicTypeBuilder_ptr &builder,
	size_t index
)
{
	builder->add_member
	(
		index,
		std::string("field_") + std::to_string(index),
		eprosima::fastrtps::types::DynamicTypeBuilderFactory::get_instance()->
			create_float128_builder()
	);
}

template<>
void buildDynamicType<bool>
(
	eprosima::fastrtps::types::DynamicTypeBuilder_ptr &builder,
	size_t index
)
{
	builder->add_member
	(
		index,
		std::string("field_") + std::to_string(index),
		eprosima::fastrtps::types::DynamicTypeBuilderFactory::get_instance()->
			create_float128_builder()
	);
}

template<>
void buildDynamicType<std::string>
(
	eprosima::fastrtps::types::DynamicTypeBuilder_ptr &builder,
	size_t index
)
{
	builder->add_member
	(
		index,
		std::string("field_") + std::to_string(index),
		eprosima::fastrtps::types::DynamicTypeBuilderFactory::get_instance()->
			create_string_builder()
	);
}

template<>
void buildDynamicType<std::string&>
(
	eprosima::fastrtps::types::DynamicTypeBuilder_ptr &builder,
	size_t index
)
{
	builder->add_member
	(
		index,
		std::string("field_") + std::to_string(index),
		eprosima::fastrtps::types::DynamicTypeBuilderFactory::get_instance()->
			create_string_builder()
	);
}

template<>
void buildDynamicType<const std::string&>
(
	eprosima::fastrtps::types::DynamicTypeBuilder_ptr &builder,
	size_t index
)
{
	builder->add_member
	(
		index,
		std::string("field_") + std::to_string(index),
		eprosima::fastrtps::types::DynamicTypeBuilderFactory::get_instance()->
			create_string_builder()
	);
}
// -----------------------------------------------------------------------------
// Command Call Listener Implementation
// -----------------------------------------------------------------------------
template <typename ret_t, typename...arg_ts>
void Command<ret_t, arg_ts...>::CommandCallListener::onNewDataMessage
(
	eprosima::fastrtps::Subscriber*
)
{
	std::cout << "command callback listener callback hit" << std::endl;
}

// =============================================================================
// Command Manager Implementation
// =============================================================================
// -----------------------------------------------------------------------------
// Constructors
// -----------------------------------------------------------------------------
CommandManager::CommandManager():
	commands()
{ }

// -----------------------------------------------------------------------------
// Implementation
// -----------------------------------------------------------------------------
template <typename ret_t, typename... arg_ts>
void CommandManager::addCommand
(
	const std::string &owner,
	const std::string &command_name,
	const std::string &docstring,
	const std::function<ret_t(arg_ts...)> &f
)
{
	this->commands.emplace_back
	(
		std::make_unique<Command<ret_t, arg_ts...>>
		(
			owner,
			command_name,
			docstring,
			f
		)
	);
}

} // end namespace dls
#endif /* end of include guard: COMMAND_TPP_OVALZHX0 */
