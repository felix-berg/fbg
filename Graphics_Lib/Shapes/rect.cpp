#include "rect.hpp"
#include "../alphacomposite.hpp"
#include "drawing_algorithms/rect.hpp"



void Rectangle::compute_stroke(Rgba * pixels, int w, int h) {
	compute_AA_rect_stroke(pixels, *this, w, h);
}


void Rectangle::compute_fill(Rgba * pixels, int w, int h) {

}
