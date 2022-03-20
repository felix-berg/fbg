#include "pixel.hpp"
#include "circle.hpp"
#include "../../alphacomposite.hpp"
#include "../../V2d.hpp"

using namespace fbg;

/*	
	Returns a boolean indicating whether the point (x, y) is
	within the bounds of (lx, ly) to (ux, uy).
	Inclusive!
*/
template <typename T>
bool fbg::in_bound(T x, T y, T lx, T ly, T ux, T uy) {
	return ! (x < lx || x > ux  ||
				 y < ly || y > uy);
}

/*
	Draw color to pixel at the given x, y coordinate.
	Doesn't draw, if the point is out of bounds.
*/
void fbg::set_pixel(Frame & frame, int x, int y, const Rgba & color) {
	if (in_bound(x, y, 0, 0, (int) frame.w - 1, (int) frame.h - 1)) 
		alpha_composite1(&frame.pixels[y * frame.w + x], &color);
}

/*
	TODO: Untested
*/
void fbg::set_pixel(Frame & frame, int x, int y, const Rgba & color, int sw) {
	if (sw == 1)
		set_pixel(frame, x, y, color);
	else
		compute_circle_fill(frame, x, y, sw, color);
}

/*
	Get the right and left pixel offsets based on given
	strokeweight
*/
void fbg::offsets_from_strokeweight(int sw, int * lOff, int * rOff) {
	if (sw % 2 == 0) { // even
		*lOff = sw / 2;
		*rOff = sw / 2 - 1;
	} else { // odd
		*lOff = (sw - 1) / 2;
		*rOff = *lOff;
	}
}

/*
	Compute pixels of given horisontal line, making sure, that no buffer overflow occurs.
	Inclusive of tx
*/
void fbg::compute_horisontal_line(Frame & frame, int fx, int fy, int tx, int ty, const Rgba & color) {
	int lLen = tx - fx + 1;		  	// length of line

	// y value
	if (fx + lLen < 0 || fx >= frame.w || fy < 0 || fy >= frame.h) {
		lLen = 0;
	} else {
		// x value
		if (fx < 0) {
			// lessen number of pixels in width (dont change the length of the line, just how many pixels are drawn of it)
			lLen += fx;
			
			// move the startindex to the first column of pixels
			fx = 0;
		}
	
		if (fx + lLen >= frame.w) 
			lLen -= (fx + lLen) - frame.w;

		int pIdx = frame.w * fy + fx; // pixel index for top left pixel of rectangle

		// composite pixels on top with the given pixel_id and line lengths
		alpha_compositeNC(&frame.pixels[pIdx], &color, lLen);
	}

}