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
#ifndef REMOTE_COMMAND_TPP_1796GJVY
#define REMOTE_COMMAND_TPP_1796GJVY

// =============================================================================
// Includes
// =============================================================================
#include "command/remote_command.hpp"
#include <fastrtps/types/DynamicData.h>

namespace dls
{
// =============================================================================
// Remote Command Implementation
// =============================================================================
// template <>
// void RemoteCommand::pushArg(char c)
// {
// 	this->remote_command_publisher.pData->set_char8_value
// 	(
// 		c,
// 		this->remote_command_publisher.command_arg_index
// 	);

// 	this->remote_command_publisher.command_arg_index++;
// }

// template <>
// void RemoteCommand::pushArg(uint8_t i)
// {
// 	this->remote_command_publisher.pData->set_uint8_value
// 	(
// 		i,
// 		this->remote_command_publisher.command_arg_index
// 	);

// 	this->remote_command_publisher.command_arg_index++;
// }

// template <>
// void RemoteCommand::pushArg(int16_t i)
// {
// 	this->remote_command_publisher.pData->set_int16_value
// 	(
// 		i,
// 		this->remote_command_publisher.command_arg_index
// 	);

// 	this->remote_command_publisher.command_arg_index++;
// }

// template <>
// void RemoteCommand::pushArg(uint16_t i)
// {
// 	this->remote_command_publisher.pData->set_uint16_value
// 	(
// 		i,
// 		this->remote_command_publisher.command_arg_index
// 	);

// 	this->remote_command_publisher.command_arg_index++;
// }

// template <>
// void RemoteCommand::pushArg(int32_t i)
// {
// 	this->remote_command_publisher.pData->set_int32_value
// 	(
// 		i,
// 		this->remote_command_publisher.command_arg_index
// 	);

// 	this->remote_command_publisher.command_arg_index++;
// }

// template <>
// void RemoteCommand::pushArg(uint32_t i)
// {
// 	this->remote_command_publisher.pData->set_uint32_value
// 	(
// 		i,
// 		this->remote_command_publisher.command_arg_index
// 	);

// 	this->remote_command_publisher.command_arg_index++;
// }

// template <>
// void RemoteCommand::pushArg(int64_t i)
// {
// 	this->remote_command_publisher.pData->set_int64_value
// 	(
// 		i,
// 		this->remote_command_publisher.command_arg_index
// 	);

// 	this->remote_command_publisher.command_arg_index++;
// }

// template <>
// void RemoteCommand::pushArg(float f)
// {
// 	this->remote_command_publisher.pData->set_float32_value
// 	(
// 		f,
// 		this->remote_command_publisher.command_arg_index
// 	);

// 	this->remote_command_publisher.command_arg_index++;
// }

// template <>
// void RemoteCommand::pushArg(double d)
// {
// 	this->remote_command_publisher.pData->set_float64_value
// 	(
// 		d,
// 		this->remote_command_publisher.command_arg_index
// 	);

// 	this->remote_command_publisher.command_arg_index++;
// }

// template <>
// void RemoteCommand::pushArg(long double ld)
// {
// 	this->remote_command_publisher.pData->set_float128_value
// 	(
// 		ld,
// 		this->remote_command_publisher.command_arg_index
// 	);

// 	this->remote_command_publisher.command_arg_index++;
// }

// template <>
// void RemoteCommand::pushArg(bool b)
// {
// 	this->remote_command_publisher.pData->set_bool_value
// 	(
// 		b,
// 		this->remote_command_publisher.command_arg_index
// 	);

// 	this->remote_command_publisher.command_arg_index++;
// }

// template <>
// void RemoteCommand::pushArg(const std::string &s)
// {
// 	this->remote_command_publisher.pData->set_string_value
// 	(
// 		s,
// 		this->remote_command_publisher.command_arg_index
// 	);

// 	this->remote_command_publisher.command_arg_index++;
// }

} // end namespace dls

#endif /* end of include guard: REMOTE_COMMAND_TPP_1796GJVY */
