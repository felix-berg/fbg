#include "pixel.hpp"
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
	Restrict a pixel id and line length based on a given point on the screen.
	Made to constrain horisontal line within frame.
*/
void restrict_pxi_ll_from_point(const V2d<int> & pt, int * pixel_idx, int * line_length, int scr_w, int scr_h) {
	// y value
	if (pt.y < 0 || pt.y >= scr_h) {
		*line_length = 0;
		return;
	}
	
	// x value
	if (pt.x < 0) {
		// lessen number of pixels in width (dont change the length of the line, just how many pixels are drawn of it)
		*line_length += pt.x + 1;

		// move the startindex to the start of the line
		*pixel_idx -=  *pixel_idx % scr_w - scr_w;
	}

	if (pt.x + *line_length >= scr_w) 
		*line_length -= (pt.x + *line_length) - scr_w;
}

/*
	Compute pixels of given horisontal line, making sure, that no buffer overflow occurs.
*/
void compute_horisontal_line(Frame & frame, V2d<int> f, V2d<int> t, const Rgba & color) {
	int px = frame.w * f.y + f.x; // pixel index for top left pixel of rectangle
	int ll = t.x - f.x;				// length of line

	// restrict pixel id and length of line within screen space
	restrict_pxi_ll_from_point(f, &px, &ll, frame.w, frame.h);
	// composite pixels on top with the given pixel_id and line lengths
	alpha_compositeNC(&frame.pixels[px], &color, ll);
}