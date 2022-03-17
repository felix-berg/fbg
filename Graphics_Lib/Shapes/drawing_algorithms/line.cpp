#include "line.hpp"
#include "../../alphacomposite.hpp"

void compute_stroke_low(Rgba * pixels, const V2d<int> & f, const V2d<int> & t, const Rgba & color, int scr_w, int scr_h);
void compute_stroke_hi(Rgba * pixels, const V2d<int> & f, const V2d<int> & t, const Rgba & color, int scr_w, int scr_h);

void set_pixel(Rgba * pixels, const V2d<int> & p, const Rgba & color, int scr_w) {
	alpha_composite1(&pixels[p.y * scr_w + p.x], &color);
}

/*
	Returns a vector of points for the pixels of the line given by points t and f.
*/
void compute_line_stroke(Rgba * pixels, const V2d<int> & f, const V2d<int> & t, const Rgba & color, int scr_w, int scr_h) {
	if (abs(t.x - f.x) > abs(t.y - f.y)) {
		if (f.x > t.x) // if points are the opposite way round
			return compute_stroke_low(pixels, t, f, color, scr_w, scr_h);
		else 
			return compute_stroke_low(pixels, f, t, color, scr_w, scr_h);
	} else 
		if (f.y > t.y)
			return compute_stroke_hi(pixels, t, f, color, scr_w, scr_h);
		else
			return compute_stroke_hi(pixels, f, t, color, scr_w, scr_h);
}


/*
	Bresenhams line algorithm: https://en.wikipedia.org/wiki/Bresenham%27s_line_algorithm
	Case for when absolute change in x is greater than absolute change in y. abs(gradient) < 1.
	Stepping along x-axis -> only one point per column
*/
void compute_stroke_low(Rgba * pixels, const V2d<int> & f, const V2d<int> & t, const Rgba & color, int scr_w, int scr_h) {
	int dx = t.x - f.x;
	int dy = t.y - f.y;

	int y_dir = 1;
	if (dy < 0) {
		y_dir = -1;
		dy *= -1;
	}
	
	// calculate the first error absolutely -> rest are calculated accumulatively
	int D  = 2 * dy - dx;
	int y  = f.y;

	// loop along x-axis: from.x -> to.y
	for (int x = f.x; x <= t.x; x++) {
		set_pixel(pixels, {x, y}, color, scr_w);
		if (D > 0) { // the point is under the line, the error is positive. Step one pixel vertically
			y += y_dir;
			D -= 2 * dx;
		}
		// no vertical step
		D += 2 * dy;
	}
}

/*
	Bresenhams line algorithm: https://en.wikipedia.org/wiki/Bresenham%27s_line_algorithm
	Case for when change in y is greater than change in x. the absolute gradient is bigger than 1
	Stepping along y-axis -> only one point per row.
	Every formula is switched from the normal "low" version
*/
void compute_stroke_hi(Rgba * pixels, const V2d<int> & f, const V2d<int> & t, const Rgba & color, int scr_w, int scr_h) {
	int dx = t.x - f.x;
	int dy = t.y - f.y;

	int x_dir = 1;
	if (dx < 0) {
		x_dir = -1;
		dx *= -1;
	}
	
	int D  = 2 * dx - dy;

	int x = f.x;

	for (int y = f.y; y <= t.y; y++) {
		set_pixel(pixels, {x, y}, color, scr_w);
		if (D > 0) {
			x += x_dir;
			D -= 2 * dy;
		}
		D += 2 * dx;
	}
}


