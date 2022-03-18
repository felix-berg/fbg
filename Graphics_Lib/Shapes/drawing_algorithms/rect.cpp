#include "../../V2d.hpp"
#include "../../rgba.hpp"
#include "rect.hpp"
#include "../../alphacomposite.hpp"

/*
	Restrict a pixel id and line length from a given point on the screen.
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
		*pixel_idx -= *pixel_idx % scr_w;
	}

	if (pt.x + *line_length >= scr_w) 
		*line_length -= (pt.x + *line_length) - scr_w;
}

/*
	Draws stroke of axis aligned rectangle r on pixel grid.
*/
void compute_AA_rect_stroke(Frame & frame, Rectangle & r) {
	const V2d<int> & tl_pt = r.pos(); 								    // point at the top left of the rectangle
	const V2d<int> & bl_pt = r.pos() + V2d<int> {0, r.height()}; // point at the bottom left of the rectangle
	const V2d<int> & tr_pt = tl_pt + V2d<int> {r.width(), 0};

	// top line
	int tl_px = frame.w * tl_pt.y + tl_pt.x; // pixel index for top left pixel of rectangle
	int tl_ll = r.width(); 					 // length of top line

	// restrict top left point and line length to be within screen space
	restrict_pxi_ll_from_point(tl_pt, &tl_px, &tl_ll, frame.w, frame.h);
	

	// if any pixels
	if (tl_ll > 0)
		alpha_compositeNC(frame.pixels + tl_px, &r.stroke(), tl_ll); // draw top line
	

	// bottom line: same thing
	int bl_px = frame.w * bl_pt.y + bl_pt.x;
	int bl_ll = r.width();

	restrict_pxi_ll_from_point(bl_pt, &bl_px, &bl_ll, frame.w, frame.h);

	if (bl_ll > 0)
		alpha_compositeNC(frame.pixels + bl_px, &r.stroke(), bl_ll); // draw bottom line

	// left line
	if (tl_pt.x >= 0 && tl_pt.x < frame.w) {
		for (int y = tl_pt.y; y < tl_pt.y + r.height() && y < frame.h; y++) {
			if (y < 0) continue; // if out of screen -> next px

			int index = tl_pt.x + y * frame.w;

			alpha_composite1(frame.pixels + index, &r.stroke());
		}
	}
	
	// right line
	if (tr_pt.x >= 0 && tr_pt.x < frame.w) {
		for (int y = tr_pt.y; y < tr_pt.y + r.height() && y < frame.h; y++) {
			if (y < 0) continue; // if out of screen -> next px

			int index = tr_pt.x + y * frame.w;

			alpha_composite1(frame.pixels + index, &r.stroke());
		}
	}
}

void compute_AA_rect_fill(Frame & f, Rectangle & r) {
	V2d<int> tl, br; // top left, bottom right
	tl = r.pos();
	br = r.pos() + V2d<int> {r.width(), r.height()};
	for (int y = tl.y; y < br.y; y++)
		alpha_compositeNC(&f.pixels[y * f.w + tl.x], &r.stroke(), r.width());
}