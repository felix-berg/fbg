#include "line.hpp"
#include "../V2d.hpp"
#include "../rgba.hpp"
#include "drawing_algorithms/line.hpp"
#include "../alphacomposite.hpp"

#include <iostream>
#include <cmath>

/*
	Change given pixelbuffer based on the pixels of the line.
*/
void Line::compute_stroke(Frame & f) {
	V2d<int> fp = from();
	V2d<int> tp = to();
	compute_line_stroke(f, fp.x, fp.y, tp.x, tp.y, stroke(), strokeweight());
}