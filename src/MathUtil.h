// a collection of mathematical utility functions

#pragma once
namespace opensaft {

template <typename T>
T Clamp(const T& value, const T& lowerBound, const T& upperBound) {
	if (value >= upperBound) return upperBound;
	if (value <= lowerBound) return lowerBound;
	return value;
}

}