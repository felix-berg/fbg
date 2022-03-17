#include "rgba.hpp"

ostream & operator << (ostream & os, const Rgba & color) {
	return os << "(r: " << (int) color.r << ", "
				 <<  "g: " << (int) color.g << ", "
				 <<  "b: " << (int) color.b << ", "
				 <<  "a: " << (int) color.a << ')';
};