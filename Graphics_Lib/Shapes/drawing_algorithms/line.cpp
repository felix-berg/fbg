#include "line.hpp"
#include "pixel.hpp"
#include "../../alphacomposite.hpp"


using namespace fbg;

void compute_stroke_low(Frame & frame, int fx, int fy, int tx, int ty, const Rgba & color, int sw);
void compute_stroke_hi(Frame & frame, int fx, int fy, int tx, int ty, const Rgba & color, int sw);

/*
	Compute the colors of the lines stroke and output onto "frame".
	Line defined by fx, fy -> tx, ty (inclusive)
*/
void fbg::compute_line_stroke(Frame & frame, int fx, int fy, int tx, int ty, const Rgba & color, int sw) {
	if (abs(tx - fx) > abs(ty - fy)) { // if gradient < 1
		if (fx > tx) // if points are the opposite way round
			compute_stroke_low(frame, tx, ty, fx, fy, color, sw);
		else 
			compute_stroke_low(frame, fx, fy, tx, ty, color, sw);
	} else // if gradient > 1, use "hi"
		if (fy > ty)
			compute_stroke_hi(frame, tx, ty, fx, fy, color, sw);
		else
			compute_stroke_hi(frame, fx, fy, tx, ty, color, sw);
}

/*
	Compute the vertical pixels for a given pixel with the correct stroke
*/
void fbg::compute_stroke_part_vertical(Frame & frame, int x, int y, const Rgba & color, int sw) {
	int loff, roff;
	offsets_from_strokeweight(sw, &loff, &roff);

	int fy = y - loff;
	int ty = y + roff;

	for (int py = fy; py <= ty; py++)
		set_pixel(frame, x, py, color);
}

/*
	Compute the horisontal pixels for a given pixel with the correct stroke
*/
void fbg::compute_stroke_part_horisontal(Frame & frame, int x, int y, const Rgba & color, int sw) {
	int loff, roff;
	offsets_from_strokeweight(sw, &loff, &roff);
	int fx = x - loff; 
	int tx = x + roff; // from x and to x
	
	compute_horisontal_line(frame, fx, y, tx, y, color);
}


/*
	Bresenhams line algorithm: https://en.wikipedia.org/wiki/Bresenham%27s_line_algorithm
	Case for when absolute change in x is greater than absolute change in y. abs(gradient) < 1.
	Stepping along x-axis -> only one point per column
*/
void compute_stroke_low(Frame & frame, int fx, int fy, int tx, int ty, const Rgba & color, int sw) {
	// create half-circle at the end and beginning of line
	if (sw % 2 == 1) { // odd
		int offset = (sw - 1) / 2;
		for (int i = 0; i < offset; i++) {
			compute_stroke_part_vertical(frame, fx - offset + i, fy, color, 2 * i + 1);
			compute_stroke_part_vertical(frame, tx + offset - i, ty, color, 2 * i + 1);
		}
	} else { // even
		int offset = sw / 2;
		for (int i = 0; i < offset; i++) {
			compute_stroke_part_vertical(frame, fx - offset + i, fy, color, 2 * i);
			compute_stroke_part_vertical(frame, tx + offset - i, ty, color, 2 * i);
		}
	}

	// bresenham line algorithm
	int dx = tx - fx;
	int dy = ty - fy;

	int y_dir = 1;
	if (dy < 0) {
		y_dir = -1;
		dy *= -1;
	}
	
	// calculate the first error absolutely -> rest are calculated accumulatively
	int D  = 2 * dy - dx;
	int y  = fy;


	// loop along x-axis: from.x -> to.y
	for (int x = fx; x <= tx; x++) {
		compute_stroke_part_vertical(frame, x, y, color, sw);
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
void compute_stroke_hi(Frame & frame, int fx, int fy, int tx, int ty, const Rgba & color, int sw) {
	// create half-circle at the end and beginning of line
	if (sw % 2 == 1) { // odd
		int offset = (sw - 1) / 2;
		for (int i = 0; i < offset; i++) {
			compute_stroke_part_horisontal(frame, fx, fy - offset + i, color, 2 * i + 1);
			compute_stroke_part_horisontal(frame, tx, ty + offset - i, color, 2 * i + 1);
		}
	} else { // even
		int offset = sw / 2;
		for (int i = 0; i < offset; i++) {
			compute_stroke_part_horisontal(frame, fx, fy - offset + i, color, 2 * i);
			compute_stroke_part_horisontal(frame, tx, ty + offset - i, color, 2 * i);
		}
	}
	
	int dx = tx - fx;
	int dy = ty - fy;

	int x_dir = 1;
	if (dx < 0) {
		x_dir = -1;
		dx *= -1;
	}
	
	int D  = 2 * dx - dy;
	int x = fx;

	for (int y = fy; y <= ty; y++) {
		compute_stroke_part_horisontal(frame, x, y, color, sw);
		if (D > 0) {
			x += x_dir;
			D -= 2 * dy;
		}
		D += 2 * dx;
	}
}


