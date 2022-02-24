#include "rgba.hpp"

ostream & operator << (ostream & os, const Rgba & color) {
	return os << "(r: " << (int) color.r << ", "
				 <<  "g: " << (int) color.g << ", "
				 <<  "b: " << (int) color.b << ", "
				 <<  "a: " << (int) color.a << ')';
};

/*
	But other over this color using premultiplied alpha compositing
*/
Rgba color_over(const Rgba & under, const Rgba & over) {
	if (over.a == 255) return over;
	unsigned char rest = 255 - over.a;
	return {
		(unsigned char) (over.r + (under.r * rest) / 255),
		(unsigned char) (over.g + (under.g * rest) / 255),
		(unsigned char) (over.b + (under.b * rest) / 255),
		(unsigned char) (over.a + (under.a * rest) / 255)
	};
}
