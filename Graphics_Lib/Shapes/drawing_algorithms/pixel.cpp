#include "pixel.hpp"
#include "circle.hpp"
#include "../../alphacomposite.hpp"
#include "../../V2d.hpp"

/*	
	Returns a boolean indicating whether the point (x, y) is
	within the bounds of (lx, ly) to (ux, uy).
	Inclusive!
*/
template <typename T>
bool in_bound(T x, T y, T lx, T ly, T ux, T uy) {
	return ! (x < lx || x > ux  ||
				 y < ly || y > uy);
}

/*
	Draw color to pixel at the given x, y coordinate.
	Doesn't draw, if the point is out of bounds.
*/
void set_pixel(Frame & frame, int x, int y, const Rgba & color) {
	if (in_bound(x, y, 0, 0, (int) frame.w - 1, (int) frame.h - 1)) 
		alpha_composite1(&frame.pixels[y * frame.w + x], &color);
}

/*
	TODO: Untested
*/
void set_pixel(Frame & frame, int x, int y, const Rgba & color, int sw) {
	if (sw == 1)
		set_pixel(frame, x, y, color);
	else
		compute_circle_fill(frame, x, y, sw, color);
}

/*
	Get the right and left pixel offsets based on given
	strokeweight
*/
void offsets_from_strokeweight(int sw, int * loff, int * roff) {
	if (sw % 2 == 0) { // even
		*loff = sw / 2;
		*roff = sw / 2 - 1;
	} else { // odd
		*loff = (sw - 1) / 2;
		*roff = *loff;
	}
}

/*
	Compute pixels of given horisontal line, making sure, that no buffer overflow occurs.
	Inclusive of tx
*/
void compute_horisontal_line(Frame & frame, int fx, int fy, int tx, int ty, const Rgba & color) {
	int pidx = frame.w * fy + fx; // pixel index for top left pixel of rectangle
	int ll = tx - fx + 1;		  	// length of line

	// y value
	if (fy < 0 || fy >= frame.h) {
		ll = 0;
		return;
	}
	
	if (fx > 1231923)
		throw std::runtime_error("");

	// x value
	if (fx < 0) {
		// lessen number of pixels in width (dont change the length of the line, just how many pixels are drawn of it)
		ll -= fx;
		

		// move the startindex to the first column of pixels
		pidx -= pidx % frame.w - frame.w;
		exit(0);
	}

	if (fx + ll >= frame.w) 
		ll -= (fx + ll) - frame.w;


	// composite pixels on top with the given pixel_id and line lengths
	alpha_compositeNC(&frame.pixels[pidx], &color, ll);
}