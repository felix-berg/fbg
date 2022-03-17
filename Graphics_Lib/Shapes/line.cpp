#include "line.hpp"
#include "../V2d.hpp"
#include "../rgba.hpp"
#include "drawing_algorithms/line.hpp"
#include "../alphacomposite.hpp"

#include <iostream>
#include <cmath>

Line::Line(const V2d<int> & f, const V2d<int> & t) 
	: Shape {{ f, t }}
{}

/*
	Change given pixelbuffer based on the pixels of the line.
*/
void Line::compute_stroke(Rgba * pixels, int width, int height) {
	compute_line_stroke(pixels, from(), to(), stroke(), width, height);
}