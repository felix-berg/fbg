#ifndef PIXELGRID_HPP
#define PIXELGRID_HPP

#include "rgba.hpp"

struct Pxgrid {
	Rgba * pixels;
	const u_int w, h;

	Pxgrid(u_int w, u_int h) {
		if (pixels != nullptr)
			throw std::runtime_error("Pixelgrid has predefined pixel pointer."); // maybe a bit paranoid

		pixels = (Rgba *) malloc(sizeof(Rgba) * w * h);
	}

	~Pxgrid() {
		if (pixels != nullptr) 
			free(pixels);
	}
};

#endif