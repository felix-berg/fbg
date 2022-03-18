#ifndef RGBA_HPP
#define RGBA_HPP

#include <iostream>
#include <SDL2/SDL.h>
using std::ostream;

/*
	A color given by a red, green, blue and alpha value.
*/
struct Rgba {
	u_char a, b, g, r; // must be opposite because of memory 

	Rgba() { };
	Rgba(u_char rr, u_char gg, u_char bb, u_char aa)
		: r { rr }, g { gg }, b { bb }, a { aa } { };
}; 

ostream & operator << (ostream & os, const Rgba & color);

#endif