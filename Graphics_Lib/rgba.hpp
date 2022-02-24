#ifndef RGBA_HPP
#define RGBA_HPP

#include <iostream>
#include <SDL2/SDL.h>
using std::ostream;

/*
	A color given by a red, green, blue and alpha value.
*/
struct Rgba {
	unsigned char a, r, g, b; 

	Rgba() { };
	Rgba(unsigned char rr, unsigned char gg, unsigned char bb, unsigned char aa)
		: r { rr }, g { gg }, b { bb }, a { aa } { };
}; 

ostream & operator << (ostream & os, const Rgba & color);

Rgba color_over(const Rgba & under, const Rgba & over);

#endif