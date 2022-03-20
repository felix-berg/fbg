#include "line.hpp"
#include "pixel.hpp"

using namespace fbg;

void draw_stroke_low(Frame & frame, int fx, int fy, int tx, int ty, const Rgba & color, int sw);
void draw_stroke_hi(Frame & frame, int fx, int fy, int tx, int ty, const Rgba & color, int sw);

/*
	Compute the colors of the lines stroke and output onto "frame".
	Line defined by fx, fy -> tx, ty (inclusive)
*/
void fbg::compute_line_stroke(Frame & frame, int fx, int fy, int tx, int ty, const Rgba & color, int sw) {
	if (abs(tx - fx) > abs(ty - fy)) { // if gradient < 1
		if (fx > tx) // if points are the opposite way round
			draw_stroke_low(frame, tx, ty, fx, fy, color, sw);
		else 
			draw_stroke_low(frame, fx, fy, tx, ty, color, sw);
	} else // if gradient > 1, use "hi"
		if (fy > ty)
			draw_stroke_hi(frame, tx, ty, fx, fy, color, sw);
		else
			draw_stroke_hi(frame, fx, fy, tx, ty, color, sw);
}

/*
	Compute the vertical pixels for a given pixel with the correct stroke
*/
void fbg::draw_stroke_part_vertical(Frame & frame, int x, int y, const Rgba & color, int sw) {
	int lOff, rOff;
	offsets_from_strokeweight(sw, &lOff, &rOff);

	int fy = y - lOff;
	int ty = y + rOff;

	for (int py = fy; py <= ty; py++)
		set_pixel(frame, x, py, color);
}

/*
	Compute the horisontal pixels for a given pixel with the correct stroke
*/
void fbg::draw_stroke_part_horisontal(Frame & frame, int x, int y, const Rgba & color, int sw) {
	int lOff, rOff;
	offsets_from_strokeweight(sw, &lOff, &rOff);
	int fx = x - lOff; 
	int tx = x + rOff; // from x and to x
	
	compute_horisontal_line(frame, fx, y, tx, y, color);
}


/*
	Bresenhams line algorithm: https://en.wikipedia.org/wiki/Bresenham%27s_line_algorithm
	Case for when absolute change in x is greater than absolute change in y. abs(gradient) < 1.
	Stepping along x-axis -> only one point per column
*/
void draw_stroke_low(Frame & frame, int fx, int fy, int tx, int ty, const Rgba & color, int sw) {
	// create half-circle at the end and beginning of line
	if (sw % 2 == 1) { // odd
		int offset = (sw - 1) / 2;
		for (int i = 0; i < offset; i++) {
			draw_stroke_part_vertical(frame, fx - offset + i, fy, color, 2 * i + 1);
			draw_stroke_part_vertical(frame, tx + offset - i, ty, color, 2 * i + 1);
		}
	} else { // even
		int offset = sw / 2;
		for (int i = 0; i < offset; i++) {
			draw_stroke_part_vertical(frame, fx - offset + i, fy, color, 2 * i);
			draw_stroke_part_vertical(frame, tx + offset - i, ty, color, 2 * i);
		}
	}

	// bresenham line algorithm
	int dx = tx - fx;
	int dy = ty - fy;

	int yDir = 1;
	if (dy < 0) {
		yDir = -1;
		dy *= -1;
	}
	
	// calculate the first error absolutely -> rest are calculated accumulatively
	int D  = 2 * dy - dx;
	int y  = fy;


	// loop along x-axis: from.x -> to.y
	for (int x = fx; x <= tx; x++) {
		draw_stroke_part_vertical(frame, x, y, color, sw);
		if (D > 0) { // the point is under the line, the error is positive. Step one pixel vertically
			y += yDir;
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
void draw_stroke_hi(Frame & frame, int fx, int fy, int tx, int ty, const Rgba & color, int sw) {
	// create half-circle at the end and beginning of line
	if (sw % 2 == 1) { // odd
		int offset = (sw - 1) / 2;
		for (int i = 0; i < offset; i++) {
			draw_stroke_part_horisontal(frame, fx, fy - offset + i, color, 2 * i + 1);
			draw_stroke_part_horisontal(frame, tx, ty + offset - i, color, 2 * i + 1);
		}
	} else { // even
		int offset = sw / 2;
		for (int i = 0; i < offset; i++) {
			draw_stroke_part_horisontal(frame, fx, fy - offset + i, color, 2 * i);
			draw_stroke_part_horisontal(frame, tx, ty + offset - i, color, 2 * i);
		}
	}
	
	int dx = tx - fx;
	int dy = ty - fy;

	int xDir = 1;
	if (dx < 0) {
		xDir = -1;
		dx *= -1;
	}
	
	int D  = 2 * dx - dy;
	int x = fx;

	for (int y = fy; y <= ty; y++) {
		draw_stroke_part_horisontal(frame, x, y, color, sw);
		if (D > 0) {
			x += xDir;
			D -= 2 * dy;
		}
		D += 2 * dx;
	}
}


