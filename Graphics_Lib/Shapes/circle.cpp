#include "drawing_algorithms/circle.hpp"


using namespace fbg;

void Circle::compute_stroke(Frame & f) {
	V2d<int> p = pos();
	int r = int(radius());
	compute_circle_stroke(f, p.x, p.y, r, stroke(), strokeweight());
}

void Circle::compute_fill(Frame & f) {
	V2d<int> p = pos();
	int r = static_cast<int>(radius());

	compute_circle_fill(f, p.x, p.y, r, fill());
}