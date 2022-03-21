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
#ifndef COMMAND_TPP_OVALZHX0
#define COMMAND_TPP_OVALZHX0

// =============================================================================
// Includes
// =============================================================================
#include "dls2/command/command.hpp"
#include "dls2/topics/command_call.hpp"
#include "dls2/topics/command_register.hpp"
#include <fastrtps/types/DynamicTypeBuilderFactory.h>
#include <fastrtps/types/DynamicDataFactory.h>
#include <fastrtps/types/DynamicData.h>
#include <iostream>
#include <thread>
#include <chrono>

#include "dls2/util/messaging/participant.hpp"

// The current release of Eigen defines in the file `Eigen_Colamd.h` the macro:
// # define ALIVE (0)
// This macro seems to be refered to in only that file. Being a very generic
// name, it clashes with the enumeration in: `eprosima::fastrtps::rtps::ALIVE`
// As of this writing, it seems that this has been fixed in Eigen's github.
// However, it has not been officially released yet. At the time of this writing
// (21 January 2020), the latest stable release of Eigen is Eigen 3.3.7,
// released on 11 December 2018. Until such time as Eigen's fix makes it into a
// new release, the following lines solves the issue.
//
// For this message, and other similar issues, refer to the wiki page `Software
// Issues`

// #ifdef ALIVE
// #undef ALIVE
// #endif

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
	//msg(buildMsg(owner_, command_name_, docstring_)),
	subscriber(
			topics::command_register,
			command_name_,
			version2::Subscriber<CommandRegisterMsgPubSubType>::CallbackType
			(
				[&](CommandRegisterMsg tuple)
				{
					std::cout << " received data" << std::endl;
					//do the magic here
				}
			)
	)
{
	//requestRegistration();
}

template <typename ret_t, typename...arg_ts>
Command<ret_t, arg_ts...>::~Command()
{
	//requestDeregistration();
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
/*template <typename ret_t, typename...arg_ts>
void Command<ret_t, arg_ts...>::requestRegistration()
{
	auto msg = this->msg;
	msg.register_nremove() = true;
	//this->publisher.publish(msg);
	std::this_thread::sleep_for(std::chrono::milliseconds(10));
}

template <typename ret_t, typename...arg_ts>
void Command<ret_t, arg_ts...>::requestDeregistration()
{
	auto msg = this->msg;
	msg.register_nremove() = false;
	//this->publisher.publish(msg);
}*/

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
/*
// -----------------------------------------------------------------------------
// Command Call Listener Constructors
// -----------------------------------------------------------------------------
template <typename ret_t, typename...arg_ts>
Command<ret_t, arg_ts...>::CommandCallListener::CommandCallListener
(
	Command<ret_t, arg_ts...> &owner_
) :
	owner(owner_),
	pSubscriber(nullptr),
	dynamic_type(),
	pData(nullptr)
{

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
		dls::impl::legacy::getFastrtpsLegacyParticipant(),
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
			dls::impl::legacy::getFastrtpsLegacyParticipant(),
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
*/

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

/*
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
		// if(info.sampleKind == eprosima::fastrtps::rtps::ALIVE)
		// {
			std::tuple<arg_ts...> tuple = buildArgTuple<arg_ts...>(0);
			owner.call(tuple);
		// }
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
*/

// =============================================================================
// Command Manager Implementation
// =============================================================================
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
