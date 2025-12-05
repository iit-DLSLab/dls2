#pragma once

#include "dls2/plugin/numerical_moving_window.hpp"

namespace dls
{

template <typename DataType>
NumericalMovingWindow<DataType>::NumericalMovingWindow(std::size_t capacity)
    : capacity_(capacity),
      data_(capacity),
      size_(0),
      head_(0),
      sum_(0.0L),
      sumSq_(0.0L),
      max_(DataType(0)),
      min_(DataType(0)) {}

template <typename DataType>
void NumericalMovingWindow<DataType>::push(DataType value) {
    if (capacity_ == 0){
		 return;  // no storage, nothing to do
	}

    // lower bound clamp to 0
    if (value < DataType(0)) {
        value = DataType(0);
    }

    if (size_ < capacity_) {
        // Not full: write at tail = (head_ + size_) % capacity_
        std::size_t pos = (head_ + size_) % capacity_;
        data_[pos] = value;
        ++size_;

        // Update aggregates
        StatType v_ld = static_cast<StatType>(value);
        sum_   += v_ld;
        sumSq_ += v_ld * v_ld;

        // First element initializes min/max
        if (size_ == 1) {
            max_ = min_ = value;
        } else {
            if (value > max_){ 
				max_ = value;
			}
            if (value < min_){ 
				min_ = value;
			}
        }
    } else {
        // Full: overwrite oldest element at head_
        DataType old = data_[head_];

        // Update aggregates for removal of old
        StatType old_ld = static_cast<StatType>(old);
        sum_   -= old_ld;
        sumSq_ -= old_ld * old_ld;

        // Overwrite
        data_[head_] = value;
        head_ = (head_ + 1) % capacity_;
        // size_ stays == capacity_

        // Add new value to aggregates
        StatType v_ld = static_cast<StatType>(value);
        sum_   += v_ld;
        sumSq_ += v_ld * v_ld;

        // Fast path: update min/max if possible
        bool needRecomputeMin = (old == min_);
        bool needRecomputeMax = (old == max_);

        // New value might become new extrema
        if (!needRecomputeMax && value > max_){
			max_ = value;
		}
        if (!needRecomputeMin && value < min_){
			min_ = value;
		}

        // If the overwritten element was an extremum, we must rescan
        if (needRecomputeMin || needRecomputeMax) {
            recomputeExtrema();
        }
    }
}

template <typename DataType>
typename NumericalMovingWindow<DataType>::StatType
NumericalMovingWindow<DataType>::mean() const noexcept {
    if (size_ == 0){ 
		return StatType(0);
	}
	return sum_ / static_cast<StatType>(size_);
}

template <typename DataType>
typename NumericalMovingWindow<DataType>::StatType
NumericalMovingWindow<DataType>::stddev() const noexcept {
    if (size_ <= 1) {
		return StatType(0);
	}
    StatType n   = static_cast<StatType>(size_);
    StatType mean   = sum_ / n;
	StatType var  = (sumSq_ - n * mean * mean) / (n - 1.0L);
    
    if (var < 0.0L){
		var = 0.0L;  // guard tiny negative due to FP
	}
	return std::sqrt(var);
}

template <typename DataType>
DataType NumericalMovingWindow<DataType>::max() const noexcept {
    if (size_ == 0) {
		return DataType(0);
	}
	return max_;
}

template <typename DataType>
DataType NumericalMovingWindow<DataType>::min() const noexcept {
    if (size_ == 0) {
		return DataType(0);
	}
	return min_;
}

template <typename DataType>
std::size_t NumericalMovingWindow<DataType>::size() const noexcept {
    return size_;
}

template <typename DataType>
std::size_t NumericalMovingWindow<DataType>::capacity() const noexcept {
    return capacity_;
}

template <typename DataType>
void NumericalMovingWindow<DataType>::clear() noexcept {
    size_  = 0;
    head_  = 0;
    sum_   = 0.0L;
    sumSq_ = 0.0L;
    // max_/min_ will be reinitialized on next push
}

template <typename DataType>
void NumericalMovingWindow<DataType>::recomputeExtrema() {
    if (size_ == 0) {
        max_ = min_ = DataType(0);
        return;
    }

    DataType localMax = data_[head_];
    DataType localMin = data_[head_];

    for (std::size_t i = 1; i < size_; ++i) {
        std::size_t pos = (head_ + i) % capacity_;
        DataType v    = data_[pos];
        if (v > localMax){ 
			localMax = v;
		}
        if (v < localMin){ 
			localMin = v;
		}
    }

    max_ = localMax;
    min_ = localMin;
}

} // end namespace dls
