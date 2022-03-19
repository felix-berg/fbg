#include "circle.hpp"
#include "../../rgba.hpp"
#include "../../alphacomposite.hpp"
#include "../../frame.hpp"
#include "pixel.hpp"
#include "line.hpp"

using namespace fbg;

/*
	Draw a point (x, y) from the first octant to every octant, with (cx, cy)
	as a referencepoint.
*/
void draw_8_octants(Frame & frame, int cx, int cy, int x, int y, const Rgba & color, int sw) {
	set_pixel(frame, cx + x, cy + y, color);	// Octants ((x, y) is normally in the "1" octant):
	set_pixel(frame, cx - x, cy + y, color);	//			  |
	set_pixel(frame, cx + x, cy - y, color);	//       8 | 7   
	set_pixel(frame, cx - x, cy - y, color);	//   4     |     3
	set_pixel(frame, cx + y, cy + x, color);	// --------+------- +x 
	set_pixel(frame, cx - y, cy + x, color);	//   2     |     1
	set_pixel(frame, cx + y, cy - x, color);	//       6 | 5
	set_pixel(frame, cx - y, cy - x, color);	//  		  | 
															// 		  +y
}

/*
	Draw lines of circle based on a point from the 
*/
void draw_circle_line_from_octant_point(Frame & frame, int cx, int cy, int x, int y, const Rgba & color) {
	// Octants ((x, y) is normally in the "1" octant):
	//			  |
	//       8 | 7 
	// 		  |  
	//   4     |     3
	// --------+------- +x 
	//   2     |     1
	// 		  |
	//       6 | 5
	//  		  | 
	// 		  +y

	// draw lines horisontally
	compute_horisontal_line(frame, cx - x + 1, cy + y, cx + x - 1, cy + y, color); // 2 -> 1
	compute_horisontal_line(frame, cx - x + 1, cy - y, cx + x - 1, cy - y, color); // 4 -> 3
	compute_horisontal_line(frame, cx - y + 1, cy + x, cx + y - 1, cy + x, color); // 6 -> 5
	compute_horisontal_line(frame, cx - y + 1, cy - x, cx + y - 1, cy - x, color); // 8 -> 7
}

/*
	Draw pixels of given circle to the given frame.
*/
void fbg::compute_circle_stroke(Frame & frame, int cx, int cy, int r, const Rgba & color, int sw) {
	int x = r;
	int y = 0;

	int D = 3 - 2 * r;

	int loff, roff;
	offsets_from_strokeweight(sw, &loff, &roff);

	while (x >= y) {
		for (int i = x - loff; i <= x + roff; i++) // take stroke weight into account
			draw_8_octants(frame, cx, cy, i, y, color, sw);

		if (D > 0) {
			D = D + 4 * (y - x) + 10;
			x = x - 1;
		}
		else // D <= 0: 
			D = D + 4 * y + 6;
		
		y = y + 1;
	}
}
/*
	Draw pixels of given circles fill to the given frame
*/
void fbg::compute_circle_fill(Frame & frame, int cx, int cy, int r, const Rgba & color) {
	int x = r;
	int y = 0;
	int D = 3 - 2 * r;

	while (x >= y) {
		draw_circle_line_from_octant_point(frame, cx, cy, x, y, color);
		if (D > 0) {
			D = D + 4 * (y - x) + 10;
			x = x - 1;
		}
		else // D <= 0: 
			D = D + 4 * y + 6;
		
		y = y + 1;
	}
}