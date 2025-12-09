
#pragma once

#include <chrono>
#include <concepts>
#include <type_traits>
#include <utility>

#include <vector>
#include <cstddef>
#include <cmath>
#include <type_traits>

namespace dls
{
	template <typename T>
	concept Arithmetic = std::is_arithmetic_v<T>;

	/**
	 * 	Generic converter from a general chrono duration to arithmetic value
	 */
	template <Arithmetic TargetType = double,
			  typename TargetPeriod = std::ratio<1>, // default: seconds
			  typename InputType,
			  typename InputPeriod>
	requires std::is_arithmetic_v<TargetType>
	constexpr TargetType toUnit(std::chrono::duration<InputType, InputPeriod> d)
	{
		using InternalTargetDuration = std::chrono::duration<long double, TargetPeriod>;
		return static_cast<TargetType>(InternalTargetDuration(d).count());
	}

	// ============================================================
	//      Converters: toSec / toMs / toUs / toNs
	//      std::chrono::duration -> arithmetic
	// ============================================================

	template <Arithmetic TargetType = double, typename Src>
	constexpr TargetType toSec(Src&& src)
	{
		return toUnit<TargetType, std::ratio<1>>(std::forward<Src>(src));
	}

	template <Arithmetic TargetType = double, typename Src>
	constexpr TargetType toMs(Src&& src)
	{
		return toUnit<TargetType, std::milli>(std::forward<Src>(src));
	}

	template <Arithmetic TargetType = double, typename Src>
	constexpr TargetType toUs(Src&& src)
	{
		return toUnit<TargetType, std::micro>(std::forward<Src>(src));
	}

	template <Arithmetic TargetType = double, typename Src>
	constexpr TargetType toNs(Src&& src)
	{
		return toUnit<TargetType, std::nano>(std::forward<Src>(src));
	}

	// ============================================================
	//      Builders: fromSec / fromMs / fromUs / fromNs
	//      arithmetic -> std::chrono::duration
	// ============================================================

	template <Arithmetic TargetType = double, Arithmetic T>
	constexpr std::chrono::duration<TargetType> fromSec(T value)
	{
		// value in seconds -> duration<TargetType, ratio<1>>
		return std::chrono::duration<TargetType>(static_cast<TargetType>(value));
	}

	template <Arithmetic TargetType = double, Arithmetic T>
	constexpr std::chrono::duration<TargetType, std::milli> fromMs(T value)
	{
		// value in milliseconds -> duration<TargetType, milli>
		return std::chrono::duration<TargetType, std::milli>(static_cast<TargetType>(value));
	}

	template <Arithmetic TargetType = double, Arithmetic T>
	constexpr std::chrono::duration<TargetType, std::micro> fromUs(T value)
	{
		// value in microseconds -> duration<TargetType, micro>
		return std::chrono::duration<TargetType, std::micro>(static_cast<TargetType>(value));
	}

	template <Arithmetic TargetType = double, Arithmetic T>
	constexpr std::chrono::duration<TargetType, std::nano> fromNs(T value)
	{
		// value in nanoseconds -> duration<TargetType, nano>
		return std::chrono::duration<TargetType, std::nano>(static_cast<TargetType>(value));
	}


} // namespace dls