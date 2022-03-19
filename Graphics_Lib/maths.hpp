#ifndef MATHS_HPP
#define MATHS_HPP

#include <cmath>

namespace fbpm {
	template <typename T>
	T random(T min, T max) {
		return (double(rand()) / double(RAND_MAX)) * double(max - min) + min;
	}
	template <typename T>
	T random(T max) {
		return (double(rand()) / double(RAND_MAX)) * double(max);
	}
}

#endif