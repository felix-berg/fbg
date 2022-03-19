#ifndef PIXELGRID_HPP
#define PIXELGRID_HPP

#include "rgba.hpp"

/*
	Container for Rgba pixelbuffer that includes width and height
*/
namespace fbg {
struct Frame { 
	Rgba * pixels;

	const int w, 
				 h;

	Frame(int w, int h) 
		: w { w }, h { h }
	{
		pixels = (Rgba *) malloc(sizeof(Rgba) * w * h);
	}

	~Frame() {
		if (pixels != nullptr) 
			free(pixels);
	}
};
};

#endif