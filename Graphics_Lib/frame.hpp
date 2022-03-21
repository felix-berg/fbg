#ifndef PIXELGRID_HPP
#define PIXELGRID_HPP

#include "rgba.hpp"

/*
	Container for Rgba pixelbuffer that includes width and height
*/
namespace fbg {
	template <typename Col>
	struct Frame { 
		Col * pixels;

		const int w, h;

		Frame(int w, int h) 
			: w { w }, h { h }
		{
			pixels = (Col *) malloc(sizeof(Col) * w * h);
			memset(pixels, 0, sizeof(Col) * w * h);
		}

		~Frame() {
			if (pixels != nullptr) 
				free(pixels);
		}
	};
};

#endif