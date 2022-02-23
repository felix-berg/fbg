#ifndef RGBA_HPP
#define RGBA_HPP

#include <ostream>
using std::ostream;

/*
	A color given by a red, green, blue and alpha value.
*/
struct Rgba {
	// has to be arranged in reverse in memory for the conversion from Rgba -> unsigned int to be correct
	unsigned char a, r, g, b; 

	Rgba() { };
	Rgba(unsigned char rr, unsigned char gg, unsigned char bb, unsigned char aa)
		: r { rr }, g { gg }, b { bb }, a { aa } { };
}; 

ostream & operator << (ostream & os, const Rgba & color);

#endif