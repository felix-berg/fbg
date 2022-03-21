#include "drawline.hpp"
#include "drawpixel.hpp"
#include "drawcircle.hpp"
#include "bresenham.hpp"

using namespace fbg;


void draw_stroke_low(Frame & frame, int fx, int fy, int tx, int ty, const Rgba & color, int sw, bool smoothEdges); 
void draw_stroke_hi(Frame & frame, int fx, int fy, int tx, int ty, const Rgba & color, int sw, bool smoothEdges);

/*
	Compute the colors of the lines stroke and output onto "frame".
	Line defined by fx, fy -> tx, ty (inclusive).
	This function doesn't make smooth edges.
*/
void fbg::compute_line_stroke(Frame & frame, int fx, int fy, int tx, int ty, const Rgba & color, int sw) {
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
	Compute the vertical pixels for a given pixel with the correct stroke
*/
void fbg::draw_stroke_part_vertical(Frame & frame, int x, int y, const Rgba & color, int sw) {
	int lOff, rOff;
	offsets_from_strokeweight(sw, &lOff, &rOff);

	int fy = y - lOff;
	int ty = y + rOff;

	for (int py = fy; py <= ty; py++)
		frame.set_pixel(x, py, color);
}

/*
	Compute the horisontal pixels for a given pixel with the correct stroke
*/
void fbg::draw_stroke_part_horisontal(Frame & frame, int x, int y, const Rgba & color, int sw) {
	int lOff, rOff;
	offsets_from_strokeweight(sw, &lOff, &rOff);
	int fx = x - lOff; 
	int tx = x + rOff; // from x and to x
	
	compute_horisontal_line(frame, fx, tx, y, color);
}


/*
	Bresenhams line algorithm: https://en.wikipedia.org/wiki/Bresenham%27s_line_algorithm
	Case for when absolute change in x is greater than absolute change in y. abs(gradient) < 1.
	Stepping along x-axis -> only one point per column
*/
void draw_stroke_low(Frame & frame, int fx, int fy, int tx, int ty, const Rgba & color, int sw, bool smoothEdges) {
	bresenham_line(fx, fy, tx, ty, [&](int x, int y) -> void {
		draw_stroke_part_vertical(frame, x, y, color, sw);
	});

	int lOff, rOff; 
	offsets_from_strokeweight(sw, &lOff, &rOff);
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

}

/*
	Bresenhams line algorithm: https://en.wikipedia.org/wiki/Bresenham%27s_line_algorithm
	Case for when change in y is greater than change in x. the absolute gradient is bigger than 1
	Stepping along y-axis -> only one point per row.
	Every formula is switched from the normal "low" version
*/
void draw_stroke_hi(Frame & frame, int fx, int fy, int tx, int ty, const Rgba & color, int sw, bool smoothEdges) {
	bresenham_line(fy, fx, ty, tx, [&](int x, int y) -> void {
		// the same as low stroke, but with opposite parameters
		draw_stroke_part_horisontal(frame, y, x, color, sw);
	});
}


