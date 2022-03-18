#include "pixel.hpp"
#include "../../alphacomposite.hpp"

/*	
	Returns a boolean indicating whether the point (x, y) is
	within the bounds of (lx, ly) to (ux, uy).
*/
template <typename T>
bool in_bound(T x, T y, T lx, T ly, T ux, T uy) {
	return ! (x < lx || x > ux ||
				 y < ly || y > uy);
}

/*
	Draw color to pixel at the given x, y coordinate.
	Doesn't draw, if the point is out of bounds.
*/
void set_pixel(Frame & frame, int x, int y, const Rgba & color) {
	if (in_bound(x, y, 0, 0, (int) frame.w, (int) frame.h))
		alpha_composite1(&frame.pixels[y * frame.w + x], &color);
}