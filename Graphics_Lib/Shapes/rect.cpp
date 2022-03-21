#include "rect.hpp"
#include "../alphacomposite.hpp"
#include "drawing_algorithms/drawrect.hpp"

using namespace fbg;

void Rect::draw_stroke(Frame & f) {
	if (!m_doStroke) return;
	V2d<int> p = pos();
	compute_AA_rect_stroke(f, p.x, p.y, width(), height(), stroke(), strokeweight());
}


void Rect::draw_fill(Frame & f) {
	if (!m_doFill) return;
	V2d<int> p = pos();
	compute_AA_rect_fill(f, p.x, p.y, width(), height(), fill());
}
