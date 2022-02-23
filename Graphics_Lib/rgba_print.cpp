#include "rgba.hpp"

ostream & operator << (ostream & os, const Rgba & color) {
	return os << "(r: " << color.r << ", "
				 << "g: " << color.g << ", "
				 << "b: " << color.b << ", "
				 << "a: " << color.a << ')';
};