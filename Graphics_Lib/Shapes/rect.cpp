#include "rect.hpp"
#include "../alphacomposite.hpp"
#include "drawing_algorithms/rect.hpp"



void Rectangle::compute_stroke(Frame & f) {
	compute_AA_rect_stroke(f, *this);
}


void Rectangle::compute_fill(Frame & f) {
	compute_AA_rect_fill(f, *this);
}
