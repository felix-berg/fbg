#include "drawing_algorithms/circle.hpp"

void Circle::compute_stroke(Frame & f) {
	compute_circle_stroke(f, *this);
}