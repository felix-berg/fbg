#ifndef RGBA_HPP
#define RGBA_HPP

#include <ostream>
using std::ostream;

/*
	A color given by a red, green, blue and alpha value.
*/
struct Rgba {
	unsigned char a, r, g, b; 

	Rgba() { };
	Rgba(unsigned char rr, unsigned char gg, unsigned char bb, unsigned char aa)
		: r { rr }, g { gg }, b { bb }, a { aa } { };

	void combine_with(const Rgba & other) { 
		unsigned char new_alpha = (255 - a) * other.a + a;
		if (new_alpha == 0) throw std::runtime_error("Error in combining colors.\n");

		this->r = ((255 - a) * other.a * other.r + a * r) / new_alpha;
		this->g = ((255 - a) * other.a * other.g + a * g) / new_alpha;
		this->b = ((255 - a) * other.a * other.b + a * b) / new_alpha;
		this->a = new_alpha;
	};
}; 

ostream & operator << (ostream & os, const Rgba & color);

#endif