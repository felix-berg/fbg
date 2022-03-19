#include "../../V2d.hpp"
#include "../../rgba.hpp"
#include "rect.hpp"
#include "line.hpp"
#include "../../alphacomposite.hpp"
#include "pixel.hpp"

using namespace fbg;

/*
	Draws stroke of axis aligned rectangle r on pixel grid.
*/
void fbg::compute_AA_rect_stroke(Frame & frame, int rx, int ry, int rw, int rh, const Rgba & color, int sw) {
	// take stroke width into account
	int loff, roff; // offsets created by strokeweight

	offsets_from_strokeweight(sw, &loff, &roff);

	int louterx = rx - loff; 			  // left x
	int routerx = rx + rw + roff;  	  // right x

	int fy = ry - loff;
	int ty = ry + roff;

	// move from y to to y
	for (int y = fy; y <= ty; y++) {
		compute_horisontal_line(frame, louterx, y, 			 routerx, y, 			  color); // top line	
		compute_horisontal_line(frame, louterx, y + rh - 1, routerx, y + rh - 1, color); // bottom line
	}


	// vertical lines
	int topy = ry - loff;
	int boty = ry + rh - 1 + roff;

	if (rx >= 0 && rx < frame.w) {
		for (int y = topy; y <= boty && y < frame.h; y++) {
			if (y < 0) continue; // if offscreen -> move to next pixel
			
			compute_stroke_part_horisontal(frame, rx, y, color, sw);
			compute_stroke_part_horisontal(frame, rx + rw, y, color, sw);
		}
	}
}

void fbg::compute_AA_rect_fill(Frame & f, int rx, int ry, int rw, int rh, const Rgba & color) {
	// bottom right point
	const int brx = rx + rw;
	const int bry = ry + rh;

	// loop from top to bottom (only inside rect)
	for (int y = ry + 1; y <= bry - 1; y++) {
		compute_horisontal_line(f, rx + 1, y, brx - 1, y, color);
	}
}