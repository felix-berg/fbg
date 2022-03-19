#include "rect.hpp"
#include "../alphacomposite.hpp"
#include "drawing_algorithms/rect.hpp"



void Rect::compute_stroke(Frame & f) {
	V2d<int> p = pos();
	compute_AA_rect_stroke(f, p.x, p.y, width(), height(), stroke(), strokeweight());
}


void Rect::compute_fill(Frame & f) {
	V2d<int> p = pos();
	compute_AA_rect_fill(f, p.x, p.y, width(), height(), fill());
}
