
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

	// ============================================================
	//      Duration converters: toSec / toMs / toUs / toNs
	//      std::chrono::duration -> arithmetic (count)
	//
	// 	e.g. double t = toMs<double>(std::chrono::system_clock::now() - prev_now);
	//
	//      Time point converters: toSec / toMs / toUs / toNs
	//      std::chrono::time_point -> arithmetic (epoch stamp)
	//
	// 	e.g. double t = toSec<double>(std::chrono::system_clock::now());
	// ============================================================

	template <Arithmetic TargetType = double,
			  typename TargetPeriod = std::ratio<1>, // default: seconds
			  typename InputType,
			  typename InputPeriod>
	constexpr TargetType toUnit(const std::chrono::duration<InputType, InputPeriod> &d)
	{
		using InternalDuration = std::chrono::duration<long double, TargetPeriod>;
		return static_cast<TargetType>(InternalDuration(d).count());
	}

	template <Arithmetic TargetType = double,
			typename TargetPeriod = std::ratio<1>,
			typename Clock,
			typename Duration>
	constexpr TargetType toUnit(const std::chrono::time_point<Clock, Duration> &tp)
	{
		return toUnit<TargetType, TargetPeriod>(tp.time_since_epoch());
	}

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
	//      Duration builders: fromSec / fromMs / fromUs / fromNs
	//      arithmetic (count) -> std::chrono::duration
	//
	// 	e.g. std::chrono::duration t = fromSec(10.2);
	// ============================================================

	template <Arithmetic TargetType = double, Arithmetic T>
	constexpr std::chrono::duration<TargetType> fromSec(T value)
	{
		return std::chrono::duration<TargetType>(static_cast<TargetType>(value));
	}

	template <Arithmetic TargetType = double, Arithmetic T>
	constexpr std::chrono::duration<TargetType, std::milli> fromMs(T value)
	{
		return std::chrono::duration<TargetType, std::milli>(static_cast<TargetType>(value));
	}

	template <Arithmetic TargetType = double, Arithmetic T>
	constexpr std::chrono::duration<TargetType, std::micro> fromUs(T value)
	{
		return std::chrono::duration<TargetType, std::micro>(static_cast<TargetType>(value));
	}

	template <Arithmetic TargetType = double, Arithmetic T>
	constexpr std::chrono::duration<TargetType, std::nano> fromNs(T value)
	{
		return std::chrono::duration<TargetType, std::nano>(static_cast<TargetType>(value));
	}

	// ============================================================
	//      Time point builders: fromSec / fromMs / fromUs / fromNs
	//      arithmetic (epoch stamp) -> std::chrono::time_point
	//
	// 	e.g. std::chrono::time_point t = fromSec<std::chrono::system_clock::time_point>(1712345678.5);
	// ============================================================

	template<typename TimePoint, Arithmetic Count, typename Period>
	constexpr TimePoint fromEpoch(Count count) noexcept
	{
		using InternalDuration = std::chrono::duration<long double, Period>;
		const auto d_in = InternalDuration{static_cast<long double>(count)};
		const auto d_out = std::chrono::duration_cast<typename TimePoint::duration>(d_in);
		return TimePoint{d_out};
	}

	template<typename TimePoint, typename Count>
	constexpr TimePoint fromSec(Count count) noexcept
	{
		return fromEpoch<TimePoint, Count, std::ratio<1>>(count);
	}

	template<typename TimePoint, typename Count>
	constexpr TimePoint fromMs(Count count) noexcept
	{
		return fromEpoch<TimePoint, Count, std::milli>(count);
	}

	template<typename TimePoint, typename Count>
	constexpr TimePoint fromUs(Count count) noexcept
	{
		return fromEpoch<TimePoint, Count, std::micro>(count);
	}

	template<typename TimePoint, typename Count>
	constexpr TimePoint fromNs(Count count) noexcept
	{
		return fromEpoch<TimePoint, Count, std::nano>(count);
	}

} // namespace dls