#include "circle.hpp"
#include "../../rgba.hpp"
#include "../../alphacomposite.hpp"
#include "../../frame.hpp"
#include "pixel.hpp"

/*
	Draw a point (x, y) from the first octant to every octant, with (cx, cy)
	as a referencepoint.
*/
void draw_8_octants(Frame & frame, int cx, int cy, int x, int y, const Rgba & color) {
	set_pixel(frame, cx + x, cy + y, color); // Octants ((x, y) is normally in the "1" octant):
	set_pixel(frame, cx - x, cy + y, color); //			 |
	set_pixel(frame, cx + x, cy - y, color); //       8 | 6   
	set_pixel(frame, cx - x, cy - y, color); //   4     |     3
	set_pixel(frame, cx + y, cy + x, color); // --------+------- +x 
	set_pixel(frame, cx - y, cy + x, color); //   2     |     1
	set_pixel(frame, cx + y, cy - x, color); //       7 | 5
	set_pixel(frame, cx - y, cy - x, color); //  		 | 
	                                         // 			 +y
}

/*
	Draw pixels of given circle to the given frame.
*/
void compute_circle_stroke(Frame & frame, Circle & c) {
	int x = c.radius();
	int y = 0;

	int cx = c.pos().x;
	int cy = c.pos().y;

	int D = 3 - 2 * c.radius();

	while (x > y) {
		draw_8_octants(frame, cx, cy, x, y, c.stroke());
		if (D > 0) {
			D = D + 4 * (y - x) + 10;
			x = x - 1;
		}
		else // D <= 0: 
			D = D + 4 * y + 6;
		
		y = y + 1;
	}
}