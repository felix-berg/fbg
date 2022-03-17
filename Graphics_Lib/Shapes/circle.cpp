#include "drawing_algorithms/circle.hpp"

void Circle::compute_stroke(Rgba * pxs, int w, int h) {
	compute_circle_stroke(pxs, *this, w, h);
}