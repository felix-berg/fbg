#ifndef PIXELGRID_HPP
#define PIXELGRID_HPP

#include "rgba.hpp"

/*
	Container for Rgba pixelbuffer that includes width and height
*/
struct Frame { 
	Rgba * pixels;

	const u_int w, 
				   h;

	Frame(u_int w, u_int h) 
		: w { w }, h { h }
	{
		pixels = (Rgba *) malloc(sizeof(Rgba) * w * h);
	}

	~Frame() {
		if (pixels != nullptr) 
			free(pixels);
	}
};

#endif