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
// =============================================================================
// Includes
// =============================================================================
#include "dls2/command/command.hpp"

namespace dls
{

// -----------------------------------------------------------------------------
// takeArg
// -----------------------------------------------------------------------------
	template <>
	ARGVOID takeArg<ARGVOID>
	(
		std::shared_ptr<eprosima::fastrtps::types::DynamicData>,
		size_t
	)
	{
		return ARGVOID();
	}
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

} // end namespace dls
