#pragma once

#include <vector>
#include <cstddef>
#include <cmath>
#include <type_traits>

namespace dls
{

/**
 * @brief Highly efficient class implementing a moving window 
 * providing basic statistical data analysis. 
 * 
 * Different numerical types are supported:
 *      e.g. int, unsigned int, float, double, long long
 * 
 * @tparam DataType 
 */
template <typename DataType = double>
class NumericalMovingWindow {

    static_assert(std::is_arithmetic_v<DataType> && !std::is_same_v<DataType, bool>,
                  "NumericalMovingWindow requires an arithmetic (non-bool) type "
                  "(int, float, double, etc.).");

public:
    using StatType = double;

    explicit NumericalMovingWindow(std::size_t capacity);

    /**
     * @brief Add a new sample (clamped to >= 0).
     * 
     * @param value 
     */
    void push(DataType value);

    /**
     * @brief Mean of samples (in high precision)
     * 
     * @return StatType 
     */
    StatType mean() const noexcept;

    /**
     * @brief Standard deviation of samples (in high precision)
     * 
     * @return StatType
     */
    StatType stddev() const noexcept;

    DataType max() const noexcept;
    DataType min() const noexcept;

    std::size_t size() const noexcept;
    std::size_t capacity() const noexcept;

    void clear() noexcept;

private:
    void recomputeExtrema();

    std::size_t          capacity_;
    std::vector<DataType>       data_;    // circular buffer storage
    std::size_t          size_;    // current number of elements (<= capacity_)
    std::size_t          head_;    // index of oldest element

    StatType            sum_;
    StatType            sumSq_;

    DataType           max_;
    DataType           min_;
};

} // end namespace dls

#include "numerical_moving_window.tpp"