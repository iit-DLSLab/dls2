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
#include "topics/command_call.hpp"
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
// -----------------------------------------------------------------------------
// Registration
// -----------------------------------------------------------------------------
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
// -----------------------------------------------------------------------------
// Calling
// -----------------------------------------------------------------------------
template <typename ret_t, typename...arg_ts>
ret_t Command<ret_t, arg_ts...>::call(std::tuple<arg_ts...> &t)
{
	static constexpr auto tuple_size = std::tuple_size<std::tuple<arg_ts...>>::value;
	return call(t, std::make_index_sequence<tuple_size>{});
}

template <typename ret_t, typename...arg_ts>
template<size_t...I>
ret_t Command<ret_t, arg_ts...>::call(std::tuple<arg_ts...> &t, std::index_sequence<I...>)
{
	return this->f(std::get<I>(t)...);
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
	participant_attributes.rtps.setName
	(
		(
			std::string(topics::command_call) + "_" +
			this->owner.owner + "_" + this->owner.command_name +
			"_sub_participant"
		)
		.c_str()
	);

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

	struct_type_builder->set_name
	(
		(
			std::string(topics::command_call) + "_" +
			this->owner.owner + "_" + this->owner.command_name +
			"_struct"
		)
		.c_str()
	);

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
	subscriber_attributes.topic.topicDataType = struct_type_builder->get_name();
	subscriber_attributes.topic.topicName =
	(
		std::string(topics::command_call) + "_" + this->owner.owner + "_" +
		this->owner.command_name
	);

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
void buildDynamicType<void>
(
	eprosima::fastrtps::types::DynamicTypeBuilder_ptr&,
	size_t
)
{ }
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
			create_string_type()
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
			create_string_type()
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
			create_string_type()
	);
}
// -----------------------------------------------------------------------------
// Command Call Listener Implementation
// -----------------------------------------------------------------------------
template <typename ret_t, typename...arg_ts>
void Command<ret_t, arg_ts...>::CommandCallListener::onNewDataMessage
(
	eprosima::fastrtps::Subscriber *sub
)
{
	eprosima::fastrtps::SampleInfo_t info;
	if(sub->takeNextData((void*)this->pData.get(), &info))
	{
		if(info.sampleKind == eprosima::fastrtps::rtps::ALIVE)
		{
			std::tuple<arg_ts...> tuple = buildArgTuple<arg_ts...>(0);
			owner.call(tuple);
		}
	}
}

// ========================== Argument Tuple Building ==========================
template <typename ret_t, typename...arg_ts>
template <typename tuple_arg1_t, typename tuple_arg2_t, typename... tuple_arg_ts>
std::tuple<tuple_arg1_t, tuple_arg2_t, tuple_arg_ts...>
	Command<ret_t, arg_ts...>::CommandCallListener::buildArgTuple
(
	size_t index
)
{
	std::cout << "template build tuple debounce CONTINUE HERE" << std::endl;
	std::tuple<tuple_arg1_t>
		t
		(
			takeArg
			<
				// typename std::remove_const
				// <
					typename std::remove_reference<tuple_arg1_t>::type
				// >::type
			>
			(pData, index)
		);
	return std::tuple_cat(t, buildArgTuple<tuple_arg2_t, tuple_arg_ts...>(++index));
}

template <typename ret_t, typename...arg_ts>
template <typename tuple_arg_t>
std::tuple<tuple_arg_t> Command<ret_t,
	arg_ts...>::CommandCallListener::buildArgTuple
(
	size_t index
)
{
	std::tuple<tuple_arg_t>
		t
		(
			takeArg
			<
				// typename std::remove_const
				// <
					typename std::remove_reference<tuple_arg_t>::type
				// >::type
			>
			(pData, index)
		);

	return t;
}

// ============================== Argument Taking ==============================
template <>
char takeArg<char>
(
	std::shared_ptr<eprosima::fastrtps::types::DynamicData> pData,
	size_t index
)
{
	char message;
	pData->get_char8_value(message, index);
	return message;
}
template <>
uint8_t takeArg<uint8_t>
(
	std::shared_ptr<eprosima::fastrtps::types::DynamicData> pData,
	size_t index
)
{
	uint8_t message;
	pData->get_uint8_value(message, index);
	return message;
}
template <>
int16_t takeArg<int16_t>
(
	std::shared_ptr<eprosima::fastrtps::types::DynamicData> pData,
	size_t index
)
{
	int16_t message;
	pData->get_int16_value(message, index);
	return message;
}
template <>
uint16_t takeArg<uint16_t>
(
	std::shared_ptr<eprosima::fastrtps::types::DynamicData> pData,
	size_t index
)
{
	uint16_t message;
	pData->get_uint16_value(message, index);
	return message;
}
template <>
int32_t takeArg<int32_t>
(
	std::shared_ptr<eprosima::fastrtps::types::DynamicData> pData,
	size_t index
)
{
	int32_t message;
	pData->get_int32_value(message, index);
	return message;
}
template <>
uint32_t takeArg<uint32_t>
(
	std::shared_ptr<eprosima::fastrtps::types::DynamicData> pData,
	size_t index
)
{
	uint32_t message;
	pData->get_uint32_value(message, index);
	return message;
}
template <>
int64_t takeArg<int64_t>
(
	std::shared_ptr<eprosima::fastrtps::types::DynamicData> pData,
	size_t index
)
{
	int64_t message;
	pData->get_int64_value(message, index);
	return message;
}
template <>
uint64_t takeArg<uint64_t>
(
	std::shared_ptr<eprosima::fastrtps::types::DynamicData> pData,
	size_t index
)
{
	uint64_t message;
	pData->get_uint64_value(message, index);
	return message;
}
template <>
float takeArg<float>
(
	std::shared_ptr<eprosima::fastrtps::types::DynamicData> pData,
	size_t index
)
{
	float message;
	pData->get_float32_value(message, index);
	return message;
}
template <>
double takeArg<double>
(
	std::shared_ptr<eprosima::fastrtps::types::DynamicData> pData,
	size_t index
)
{
	double message;
	pData->get_float64_value(message, index);
	return message;
}
template <>
long double takeArg<long double>
(
	std::shared_ptr<eprosima::fastrtps::types::DynamicData> pData,
	size_t index
)
{
	long double message;
	pData->get_float128_value(message, index);
	return message;
}
template <>
bool takeArg<bool>
(
	std::shared_ptr<eprosima::fastrtps::types::DynamicData> pData,
	size_t index
)
{
	bool message;
	pData->get_bool_value(message, index);
	return message;
}
template <>
std::string takeArg<std::string>
(
	std::shared_ptr<eprosima::fastrtps::types::DynamicData> pData,
	size_t index
)
{
	std::string message;
	pData->get_string_value(message, index);
	return message;
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
