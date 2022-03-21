#include "drawline.hpp"
#include "drawpixel.hpp"
#include "drawcircle.hpp"

using namespace fbg;

void draw_stroke_low(Frame<Rgba> & frame, int fx, int fy, int tx, int ty, const Rgba & color, int sw, bool smoothEdges);

template<typename Col>
void draw_stroke_hi(Frame<Col> & frame, int fx, int fy, int tx, int ty, const Col & color, int sw, bool smoothEdges);

/*
	Compute the colors of the lines stroke and output onto "frame".
	Line defined by fx, fy -> tx, ty (inclusive).
	This function doesn't make smooth edges.
*/
void fbg::compute_line_stroke(Frame<Rgba> & frame, int fx, int fy, int tx, int ty, const Rgba & color, int sw) {
	if (abs(tx - fx) > abs(ty - fy)) { // if gradient < 1
		if (fx > tx) // if points are the opposite way round
			draw_stroke_low(frame, tx, ty, fx, fy, color, sw, false);
		else 
			draw_stroke_low(frame, fx, fy, tx, ty, color, sw, false);
	} else // if gradient > 1, use "hi"
		if (fy > ty)
			draw_stroke_hi(frame, tx, ty, fx, fy, color, sw, false);
		else
			draw_stroke_hi(frame, fx, fy, tx, ty, color, sw, false);
}

/*
	Draws a line from (fx, fy) to (tx, ty) with smooth edges (small circles).
*/
void fbg::compute_line_stroke_smooth(Frame<Rgba> & frame, int fx, int fy, int tx, int ty, const Rgba & color, int sw) {
	if (abs(tx - fx) > abs(ty - fy)) { // if gradient < 1
		if (fx > tx) // if points are the opposite way round
			draw_stroke_low(frame, tx, ty, fx, fy, color, sw, true);
		else 
			draw_stroke_low(frame, fx, fy, tx, ty, color, sw, true);
	} else // if gradient > 1, use "hi"
		if (fy > ty)
			draw_stroke_hi(frame, tx, ty, fx, fy, color, sw, true);
		else
			draw_stroke_hi(frame, fx, fy, tx, ty, color, sw, true);
}

/*
	Compute the vertical pixels for a given pixel with the correct stroke
*/
void fbg::draw_stroke_part_vertical(Frame<Rgba> & frame, int x, int y, const Rgba & color, int sw) {
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
void fbg::draw_stroke_part_horisontal(Frame<Rgba> & frame, int x, int y, const Rgba & color, int sw) {
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
void draw_stroke_low(Frame<Rgba> & frame, int fx, int fy, int tx, int ty, const Rgba & color, int sw, bool smoothEdges) {
	int lOff, rOff; 
	offsets_from_strokeweight(sw, &lOff, &rOff);

	// bresenham line algorithm
	int dx = tx - fx;
	int dy = ty - fy;

	int yDir = 1;

	if (dy < 0) {
		yDir = -1;
		dy *= -1;
	}

	// 0 0 0 l 0 0   0 0 c c 0 0   0 0 0 l 0 0
	// 0 0 0 l 0 0   0 c 0 0 c 0   0 0 0 l c 0 
	// 0 0 0 l 0 0   c 0 0 0 0 c   0 0 0 l 0 c 
	// 0 0 l 0 0 0 + c 0 0 0 0 c = 0 0 l 0 0 c
	// 0 0 l 0 0 0	  c 0 0 0 0 c   0 0 l 0 0 c
	// 0 l 0 0 0 0   0 c 0 0 c 0   0 l 0 0 c 0 
	// 0 l 0 0 0 0   0 0 c c 0 0   0 l c c 0 0 

	// Frame<bool> end_circle { sw, sw };
	// compute_circle_fill(end_circle, sw / 2, sw / 2, sw / 2, true);
	
	// for (int j = 0; j < end_circle.h; j++) {
	// 	for (int i = 0; i < end_circle.w; i++)
	// 		std::cout << (int) end_circle.pixels[j * end_circle.w + i] << ' ';
	// 	std::cout << '\n';
	// }
	// std::cout << '\n';

	
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
template <typename Col>
void draw_stroke_hi(Frame<Col> & frame, int fx, int fy, int tx, int ty, const Col & color, int sw, bool smoothEdges) {
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


